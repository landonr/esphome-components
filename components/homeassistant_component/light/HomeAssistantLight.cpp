#include "HomeAssistantLight.h"
#include <set>
#include <sstream>
#include <utility>
#include "../LightExtensions.h"
#include "esphome/core/log.h"

namespace esphome {
namespace homeassistant_light {

static const char* const TAG = "homeassistant.light";

void HomeAssistantLight::subscribe_states() {
  ESP_LOGI(TAG, "'%s': Subscribe states", get_name().c_str());

  subscribe_homeassistant_state(&HomeAssistantLight::state_changed,
                                this->entity_id_);
  subscribe_homeassistant_state(&HomeAssistantLight::min_mireds_changed,
                                this->entity_id_, "min_mireds");
  subscribe_homeassistant_state(&HomeAssistantLight::max_mireds_changed,
                                this->entity_id_, "max_mireds");
  subscribe_homeassistant_state(&HomeAssistantLight::brightness_changed,
                                this->entity_id_, "brightness");
  subscribe_homeassistant_state(&HomeAssistantLight::color_temp_changed,
                                this->entity_id_, "color_temp");
  subscribe_homeassistant_state(&HomeAssistantLight::color_changed,
                                this->entity_id_, "hs_color");
  subscribe_homeassistant_state(&HomeAssistantLight::color_mode_changed,
                                this->entity_id_, "color_mode");
  subscribe_homeassistant_state(
      &HomeAssistantLight::supported_color_modes_changed, this->entity_id_,
      "supported_color_modes");
}

void HomeAssistantLight::setup() {
  ESP_LOGI(TAG, "'%s': Setup", get_name().c_str());
  subscribe_states();
}

light::LightTraits HomeAssistantLight::get_traits() {
  return light_traits_;
}

void HomeAssistantLight::setup_state(light::LightState* state) {
  ESP_LOGI(TAG, "'%s': Setting up state", get_name().c_str());
  light_state_ = state;
}

void HomeAssistantLight::update_state(light::LightState* state) {
  bool is_on = state->remote_values.is_on();
  auto brightness =
      static_cast<int>(state->remote_values.get_brightness() * 255);
  auto color_temp =
      static_cast<int>(state->remote_values.get_color_temperature());
  ESP_LOGD(TAG, "'%s': Update state %d %d %d", get_name().c_str(), is_on,
           brightness, color_temp);
}

void HomeAssistantLight::set_color_properties(
    std::map<std::string, std::string>* data, light::LightState* state,
    const light::ColorMode color_mode) {
  switch (color_mode) {
    case light::ColorMode::UNKNOWN:
    case light::ColorMode::ON_OFF:
    case light::ColorMode::BRIGHTNESS:
    case light::ColorMode::WHITE:
      break;
    case light::ColorMode::COLOR_TEMPERATURE:
    case light::ColorMode::RGB_COLOR_TEMPERATURE:
    case light::ColorMode::COLD_WARM_WHITE: {
      auto color_temp =
          static_cast<int>(state->remote_values.get_color_temperature());
      if (color_temp != 0) {
        (*data)["color_temp"] = to_string(color_temp);
      }
      break;
    };
    case light::ColorMode::RGB:
    case light::ColorMode::RGB_WHITE:
    case light::ColorMode::RGB_COLD_WARM_WHITE: {
      (*data)["hue"] = to_string(get_hsv_color(state));
      (*data)["saturation"] = "100";
      break;
    }
  }
}

void HomeAssistantLight::publish_api_state(light::LightState* state) {
  ignore_api_updates_with_seconds(2);
  ESP_LOGI(TAG, "publish_api_state: %s: new output state",
           state->get_name().c_str());
  if (state->remote_values.is_on()) {
    std::map<std::string, std::string> data = {
        {"entity_id", entity_id_.c_str()}, {"transition", "0"}};
    auto brightness =
        static_cast<int>(state->remote_values.get_brightness() * 255);
    auto color_mode = state->remote_values.get_color_mode();

    if (brightness != 0) {
      data["brightness"] = to_string(brightness);
    }
    set_color_properties(&data, state, color_mode);
    if (color_mode == light::ColorMode::RGB_WHITE ||
        color_mode == light::ColorMode::RGB ||
        color_mode == light::ColorMode::RGB_COLD_WARM_WHITE) {
      call_homeassistant_service("script.hs_light_set", data);
    } else {
      call_homeassistant_service("light.turn_on", data);
    }
  } else {
    const std::map<std::string, std::string> data = {
        {"entity_id", entity_id_.c_str()},
    };
    call_homeassistant_service("light.turn_off", data);
  }
}

void HomeAssistantLight::write_state(light::LightState* state) {
  ESP_LOGI(TAG, "'%s': Writing state %d - publish api state: %d",
           get_name().c_str(), state->remote_values.is_on(),
           can_publish_to_api());
  if (can_publish_to_api()) {
    publish_api_state(state);
    ignore_publish_with_seconds(0.5);
  }
  this->state_callback_.call();
}

void HomeAssistantLight::add_on_state_callback(
    std::function<void()>&& callback) {
  this->state_callback_.add(std::move(callback));
}

void HomeAssistantLight::update_color_with_hsv(const float hsv_color) {
  auto call = this->light_state_->make_call();
  if (light_traits_.supports_color_mode(light::ColorMode::RGB)) {
    call.set_color_mode(light::ColorMode::RGB);
  } else if (light_traits_.supports_color_mode(light::ColorMode::RGB_WHITE)) {
    call.set_color_mode(light::ColorMode::RGB_WHITE);
  } else {
    ESP_LOGI(TAG, "'%s': doesnt support color!", get_name().c_str());
    return;
  }
  if (!light_state_->remote_values.is_on()) {
    call.set_state(true);
    call.set_brightness(0.1);
  }
  float red, green, blue;
  hsv_to_rgb(hsv_color, 1, 1, red, green, blue);
  call.set_rgb(red, green, blue);
  call.perform();
}

void HomeAssistantLight::setAttribute(
    const std::map<std::string, std::string>& data) {
  // call_homeassistant_service("light.turn_on", data);
}

void HomeAssistantLight::state_changed(std::string state) {
  auto onState =
      parse_on_off(state.c_str()) == esphome::ParseOnOffState::PARSE_ON;
  ESP_LOGI(TAG,
           "state_changed: '%s': (write %d) state changed to %s onState is %d "
           "- publish disabled",
           get_name().c_str(), can_update_from_api(), state.c_str(), onState);

  ignore_publish_with_seconds(1);
  auto call = this->light_state_->make_call();
  call.set_state(onState);
  if (!onState) {
    call.set_brightness(0);
  }
  call.perform();
}
void HomeAssistantLight::min_mireds_changed(std::string state) {
  ESP_LOGI(TAG, "'%s': (write %d) min_mireds changed to %s", get_name().c_str(),
           can_update_from_api(), state.c_str());
  auto min_mireds = atoi(state.c_str());
  light_traits_.set_min_mireds(min_mireds);
}
void HomeAssistantLight::max_mireds_changed(std::string state) {
  ESP_LOGI(TAG, "'%s': (write %d) max_mireds changed to %s", get_name().c_str(),
           can_update_from_api(), state.c_str());
  auto max_mireds = atoi(state.c_str());
  light_traits_.set_max_mireds(max_mireds);
}
void HomeAssistantLight::brightness_changed(std::string state) {
  ESP_LOGI(TAG, "'%s': (write %d) brightness changed to %s", get_name().c_str(),
           can_update_from_api(), state.c_str());
  auto brightness = parse_number<float>(state);
  update_supported_color_mode(light::ColorMode::BRIGHTNESS);

  if (can_update_from_api() && brightness.has_value()) {
    ignore_publish_with_seconds(1);
    auto call = this->light_state_->make_call();
    call.set_brightness(brightness.value() / this->max_value_);
    call.perform();
  }
}
void HomeAssistantLight::color_temp_changed(std::string state) {
  ESP_LOGI(TAG, "'%s': (write %d) color_temp changed to %s", get_name().c_str(),
           can_update_from_api(), state.c_str());
  update_supported_color_mode(light::ColorMode::COLOR_TEMPERATURE);

  auto color_temp = parse_number<int>(state);
  if (can_update_from_api() && color_temp.has_value()) {
    ignore_publish_with_seconds(1);
    auto call = this->light_state_->make_call();
    call.set_color_mode(light::ColorMode::COLOR_TEMPERATURE);
    call.set_color_temperature(color_temp.value());
    call.perform();
  }
}

int HomeAssistantLight::extractFirstNumber(std::string input) {
  // Remove the opening and closing parentheses
  input = input.substr(1, input.size() - 2);
  // Split the string on the comma
  std::istringstream iss(input);
  std::string first, second;
  std::getline(iss, first, ',');
  // Convert the first part to a int
  int num = std::stod(first);
  return num;
}

void HomeAssistantLight::color_changed(std::string state) {
  ESP_LOGI(TAG, "'%s': (write %d) color changed to %s", get_name().c_str(),
           can_update_from_api(), state.c_str());

  update_supported_color_mode(light::ColorMode::RGB_WHITE);
  if (can_update_from_api()) {
    ignore_publish_with_seconds(1);
    if (state.find("None") != std::string::npos) {
      ESP_LOGI(TAG, "'%s': (write %d) color changed to None",
               get_name().c_str(), can_update_from_api());
      this->light_state_->turn_off().perform();
      return;
    }
    auto localColor = extractFirstNumber(state);
    float red, green, blue;
    hsv_to_rgb(localColor, 1, 1, red, green, blue);
    auto call = this->light_state_->make_call();
    call.set_color_mode(light::ColorMode::RGB_WHITE);
    call.set_rgb(red, green, blue);
    call.perform();
  }
}

optional<light::ColorMode> HomeAssistantLight::parse_color_mode(
    std::string color_mode) {
  if (color_mode.find("onoff") != std::string::npos) {
    ESP_LOGI(TAG, "Parsed color mode: ON_OFF");
    return light::ColorMode::ON_OFF;
  } else if (color_mode.find("brightness") != std::string::npos) {
    ESP_LOGI(TAG, "Parsed color mode: BRIGHTNESS");
    return light::ColorMode::BRIGHTNESS;
  } else if (color_mode.find("color_temp") != std::string::npos) {
    ESP_LOGI(TAG, "Parsed color mode: COLOR_TEMPERATURE");
    return light::ColorMode::COLOR_TEMPERATURE;
  } else if (color_mode.find("hs") != std::string::npos) {
    ESP_LOGI(TAG, "Parsed color mode: COLOR_TEMPERATURE");
    return light::ColorMode::COLOR_TEMPERATURE;
  } else if (color_mode.find("rgbww") != std::string::npos) {
    ESP_LOGI(TAG, "Parsed color mode: RGB_COLD_WARM_WHITE");
    return light::ColorMode::RGB_COLD_WARM_WHITE;
  } else if (color_mode.find("rgbw") != std::string::npos) {
    ESP_LOGI(TAG, "Parsed color mode: RGB_WHITE");
    return light::ColorMode::RGB_WHITE;
  } else if (color_mode.find("rgb") != std::string::npos) {
    ESP_LOGI(TAG, "Parsed color mode: RGB");
    return light::ColorMode::RGB;
  } else if (color_mode.find("white") != std::string::npos) {
    ESP_LOGI(TAG, "Parsed color mode: WHITE");
    return light::ColorMode::WHITE;
  } else if (color_mode.find("xy") != std::string::npos) {
    ESP_LOGI(TAG, "Parsed color mode: RGB_WHITE");
    return light::ColorMode::RGB_WHITE;
  }
  ESP_LOGI(TAG, "Failed to parse color mode");
  return {};
}

void HomeAssistantLight::update_supported_color_mode(light::ColorMode mode) {
  auto supported_modes = light_traits_.get_supported_color_modes();
  if (!supported_modes.count(mode)) {
    supported_modes.insert(mode);
    light_traits_.set_supported_color_modes(supported_modes);
  }
}

void HomeAssistantLight::color_mode_changed(std::string state) {
  ESP_LOGI(TAG, "'%s': (write %d) color_mode changed changed to %s",
           get_name().c_str(), can_update_from_api(), state.c_str());
  auto parsed_color_mode = parse_color_mode(state);
  if (can_update_from_api() && parsed_color_mode.has_value()) {
    ignore_publish_with_seconds(1);
    update_supported_color_mode(parsed_color_mode.value());
    auto call = this->light_state_->make_call();
    call.set_color_mode(parsed_color_mode.value());
    call.perform();
  }
}

std::vector<std::string> HomeAssistantLight::split(const std::string& s,
                                                   const std::string& delim) {
  size_t pos = 0;
  std::vector<std::string> elems;
  std::string token;
  std::string sCopy = s;
  while ((pos = sCopy.find(delim)) != std::string::npos) {
    elems.push_back(s.substr(0, pos));
    sCopy.erase(0, pos + delim.length());
  }
  // add rest of string to not miss the last part
  elems.push_back(sCopy);
  return elems;
}

void HomeAssistantLight::supported_color_modes_changed(std::string state) {
  ESP_LOGI(TAG, "'%s': (write %d) supported_color_modes_changed changed to %s",
           get_name().c_str(), can_update_from_api(), state.c_str());
  auto modes = split(state, ",");
  std::set<light::ColorMode> supportedModes;
  for (auto cmode : modes) {
    auto parsed_color_mode = parse_color_mode(cmode);
    if (parsed_color_mode.has_value()) {
      supportedModes.insert(parsed_color_mode.value());
    }
  }
  for (auto mode : supportedModes) {
    ESP_LOGI(
        TAG, "'%s': (write %d) supported_color_modes_changed changed to %d",
        get_name().c_str(), can_update_from_api(), static_cast<uint8_t>(mode));
  }
  light_traits_.set_supported_color_modes(supportedModes);
}

bool HomeAssistantLight::get_state() {
  return light_state_->remote_values.is_on();
}

}  // namespace homeassistant_light
}  // namespace esphome
