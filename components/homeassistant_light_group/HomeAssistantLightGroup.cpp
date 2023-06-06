#include "HomeAssistantLightGroup.h"
#include "LightExtensions.h"
#include "esphome/core/log.h"

namespace esphome {
namespace homeassistant_light_group {

static const char* const TAG = "homeassistant.light.group";

bool HomeAssistantLightGroup::selectLightDetailAtIndex(int index) {
  if (lights.size() < index) {
    ESP_LOGE(TAG, "selecting out of bounds light");
    return false;
  }
  auto new_active_light = lights[index];
  auto output =
      static_cast<light::LightOutput*>(new_active_light->get_output());
  if (supportsBrightness(new_active_light)) {
    _activeLight = new_active_light;
    return true;
  }
  return false;
}

bool HomeAssistantLightGroup::selectLightDetail(
    light::LightState* new_active_light) {
  auto output =
      static_cast<light::LightOutput*>(new_active_light->get_output());
  if (supportsBrightness(new_active_light)) {
    _activeLight = new_active_light;
    return true;
  }
  return false;
}

void HomeAssistantLightGroup::register_light(light::LightState* newLight) {
  lights.push_back(newLight);
#ifdef USE_API_LIGHT
  auto output = static_cast<homeassistant_light::HomeAssistantLight*>(
      newLight->get_output());
  if (output) {
    output->add_on_state_callback([this, newLight]() {
      ESP_LOGI(TAG, "'%s': new output state", newLight->get_name().c_str());
      this->publish_state(0);
    });
  }
#endif
}

void HomeAssistantLightGroup::toggleLight(int index) {
  if (lights.size() < index) {
    return;
  }
  auto light = lights[index];
  if (!light->remote_values.is_on() &&
      light->remote_values.get_brightness() == 0) {
    auto call = light->make_call();
    call.set_state(true);
    call.set_brightness(1.0);
    auto traits = light->get_traits();
    if (traits.supports_color_mode(light::ColorMode::RGB)) {
      call.set_color_mode(light::ColorMode::RGB);
      call.set_rgb(1, 1, 1);
    } else if (traits.supports_color_mode(light::ColorMode::RGB_WHITE)) {
      call.set_color_mode(light::ColorMode::RGB_WHITE);
      call.set_rgbw(1, 1, 1, 1);
    }
    call.perform();
    return;
  }
  light->toggle().perform();
}

void HomeAssistantLightGroup::decTemperature() {
  auto activeLight = getActiveLight();
  if (activeLight) {
    auto call = activeLight->make_call();
    call.set_color_mode(light::ColorMode::COLOR_TEMPERATURE);
    if (!activeLight->remote_values.is_on()) {
      call.set_state(true);
      call.set_brightness(0.1);
    }
    auto traits = activeLight->get_traits();
    float mired_step =
        static_cast<float>(traits.get_max_mireds() - traits.get_min_mireds()) /
        20.0;
    float color_temperature =
        activeLight->remote_values.get_color_temperature() - mired_step;
    call.set_color_temperature(
        std::max(traits.get_min_mireds(), color_temperature));
    call.perform();
  }
}

void HomeAssistantLightGroup::incTemperature() {
  auto activeLight = getActiveLight();
  if (activeLight) {
    auto call = activeLight->make_call();
    call.set_color_mode(light::ColorMode::COLOR_TEMPERATURE);
    if (!activeLight->remote_values.is_on()) {
      call.set_state(true);
      call.set_brightness(0.1);
    }
    auto traits = activeLight->get_traits();
    float mired_step =
        static_cast<float>(traits.get_max_mireds() - traits.get_min_mireds()) /
        20.0;
    float color_temperature =
        activeLight->remote_values.get_color_temperature() + mired_step;
    call.set_color_temperature(
        std::min(traits.get_max_mireds(), color_temperature));
    call.perform();
  }
}
void HomeAssistantLightGroup::decBrightness() {
  auto activeLight = getActiveLight();
  if (activeLight) {
    auto call = activeLight->make_call();
    auto brightness = activeLight->remote_values.get_brightness() - 0.1;
    if (brightness > 0) {
      call.set_brightness(brightness);
      ESP_LOGI(TAG, "'%s': brightness decreased to %f",
               activeLight->get_name().c_str(), brightness);
    } else {
      call.set_state(false);
      ESP_LOGI(TAG, "'%s': brightness decreased to off",
               activeLight->get_name().c_str());
    }
    call.perform();
  }
}
void HomeAssistantLightGroup::incBrightness() {
  auto activeLight = getActiveLight();
  if (activeLight) {
    auto call = activeLight->make_call();
    if (activeLight->remote_values.is_on()) {
      auto brightness = activeLight->remote_values.get_brightness() + 0.1;
      call.set_brightness(std::min(1.0, brightness));
      ESP_LOGI(TAG, "'%s': brightness increased to %f",
               activeLight->get_name().c_str(), brightness);
    } else {
      call.set_state(true);
      call.set_brightness(0.1);
      ESP_LOGI(TAG, "'%s': brightness set to %f",
               activeLight->get_name().c_str(), 0.1);
    }
    call.perform();
  }
}
void HomeAssistantLightGroup::toggle() {
  auto activeLight = getActiveLight();
  if (activeLight) {
    activeLight->toggle().perform();
  }
}
void HomeAssistantLightGroup::decColor() {
  auto activeLight = getActiveLight();
  if (activeLight) {
    float color_step = 360.0f / 20.0f;
    float hsv_color = std::max(0.1f, get_hsv_color(activeLight) - color_step);
    update_color_with_hsv(hsv_color, activeLight);
  }
}
void HomeAssistantLightGroup::incColor() {
  auto activeLight = getActiveLight();
  if (activeLight) {
    float color_step = 360.0f / 20.0f;
    float hsv_color = std::min(359.9f, get_hsv_color(activeLight) + color_step);
    update_color_with_hsv(hsv_color, activeLight);
  }
}

void HomeAssistantLightGroup::update_color_with_hsv(const float hsv_color,
                                                    light::LightState* light) {
  auto call = light->make_call();
  auto traits = light->get_traits();
  if (traits.supports_color_mode(light::ColorMode::RGB)) {
    call.set_color_mode(light::ColorMode::RGB);
  } else if (traits.supports_color_mode(light::ColorMode::RGB_WHITE)) {
    call.set_color_mode(light::ColorMode::RGB_WHITE);
  } else {
    ESP_LOGI(TAG, "'%s': doesnt support color!", light->get_name().c_str());
    return;
  }
  if (!light->remote_values.is_on()) {
    call.set_state(true);
    call.set_brightness(0.1);
  }
  float red, green, blue;
  hsv_to_rgb(hsv_color, 1, 1, red, green, blue);
  call.set_rgb(red, green, blue);
  call.perform();
}
}  // namespace homeassistant_light_group
}  // namespace esphome
