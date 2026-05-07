#include <map>
#include <string>
#include "esphome/components/fan/fan.h"
#include "esphome/components/homeassistant_component/HomeAssistantComponent.h"

namespace esphome {
namespace homeassistant_fan {
static const char* const TAG = "homeassistant.fan";

class HomeAssistantFan
    : public fan::Fan,
      public homeassistant_component::HomeAssistantComponent {
  int percentage = 0;
  int percentage_step = 0;

  void setup() {
    ESP_LOGI(TAG, "'%s': Setup", get_name().c_str());
    subscribe_homeassistant_state(&HomeAssistantFan::state_changed,
                                  this->entity_id_);
    subscribe_homeassistant_state(&HomeAssistantFan::percentage_changed,
                                  this->entity_id_, "percentage");
    subscribe_homeassistant_state(&HomeAssistantFan::percentage_step_changed,
                                  this->entity_id_, "percentage_step");
  }

  void publish_api_state(fan::FanCall call) {
    next_api_publish_ = false;
    ignore_api_updates_with_seconds(2);
    ESP_LOGI(TAG, "publish_api_state: '%s'", this->get_name().c_str());
    std::map<std::string, std::string> data = {{"entity_id", entity_id_}};

    if (call.get_state().has_value() && *call.get_state()) {
      // if (call.get_oscillating()) {
      //   data.insert({"oscilating", "true"});
      // }
      if (call.get_speed().has_value()) {
        int percentage = *call.get_speed() * percentage_step;
        ESP_LOGI(TAG, "publish_api_state: '%s' set speed to %d", this->get_name().c_str(), percentage);
        data["percentage"] = to_string(percentage);
        call_homeassistant_service("fan.set_percentage", data);
      } else {
        call_homeassistant_service("fan.turn_on", data);
        ESP_LOGI(TAG, "publish_api_state: '%s' turn on", this->get_name().c_str());
      }
    } else {
      call_homeassistant_service("fan.turn_off", data);
      ESP_LOGI(TAG, "publish_api_state: '%s' turn off", this->get_name().c_str());
    }
    next_api_publish_ = true;
    // if (call.get_direction().has_value()) {
    //   data["direction"] = *call.get_direction() ? "forward" : "reverse";
    // }
    // if (call.get_stop()) {
    //   call_homeassistant_service("fan.stop", data);
    // }
    // if (call.get_toggle().has_value()) {
    //   call_homeassistant_service("fan.toggle", data);
    // }
    // if (call.get_position().has_value()) {
    //   data["position"] = to_string(*call.get_position());
    //   call_homeassistant_service("fan.set_position", data);
    // }

    // if (call.get_stop()) {
    //   ESP_LOGI(TAG, "control: '%s' stopping", this->get_name().c_str());
    //   std::map<std::string, std::string> data = {{"entity_id", entity_id_}};
    //   call_homeassistant_service("fan.stop", data);
    // } else
    // if (call.get_toggle().has_value()) {
    //   if (this->current_operation != fan::COVER_OPERATION_IDLE) {
    //     ESP_LOGI(TAG, "control: '%s' toggle", this->get_name().c_str());
    //     std::map<std::string, std::string> data = {{"entity_id", entity_id_}};
    //     call_homeassistant_service("fan.toggle", data);
    //   } else {
    //     if (this->position == fan::COVER_CLOSED) {
    //       // this->last_operation_ == fan::COVER_OPERATION_CLOSING) {
    //       ESP_LOGI(TAG, "control: '%s' opening", this->get_name().c_str());
    //       std::map<std::string, std::string> data = {{"entity_id", entity_id_}};
    //       call_homeassistant_service("fan.open_fan", data);
    //     } else {
    //       ESP_LOGI(TAG, "control: '%s' closing", this->get_name().c_str());
    //       std::map<std::string, std::string> data = {{"entity_id", entity_id_}};
    //       call_homeassistant_service("fan.close_fan", data);
    //     }
    //   }
    // } else if (call.get_position().has_value()) {
    //   auto pos = *call.get_position();
    //   if (pos == this->position) {
    //     // already at target
    //   } else {
    //     ESP_LOGI(TAG, "control: '%s' stopping", this->get_name().c_str());
    //     std::map<std::string, std::string> data = {
    //         {"entity_id", entity_id_}, {"position", to_string(pos)}};
    //     call_homeassistant_service("fan.stop", data);
    //   }
    // }
  }

  void state_changed(std::string state) {
    next_api_publish_ = false;
    
    if (can_update_from_api()) {
      if (strcmp(state.c_str(), "on") == 0) {
        auto call = this->turn_on();
        call.perform();
        ESP_LOGI(TAG, "state_changed: '%s' to on: %s", this->get_name().c_str(),
                 state.c_str());
      } else if (strcmp(state.c_str(), "off") == 0) {
        auto call = this->turn_off();
        call.perform();
        ESP_LOGI(TAG, "state_changed: '%s' to off: %s",
                 this->get_name().c_str(), state.c_str());
      } else {
        ESP_LOGW(TAG, "state_changed: '%s' to unknown: %s",
                 this->get_name().c_str(), state.c_str());
      }
    }
    next_api_publish_ = true;
  }

  void percentage_changed(std::string state) {
    next_api_publish_ = false;
    ESP_LOGI(TAG, "percentage_changed: '%s' to %s", this->get_name().c_str(),
             state.c_str());
    percentage = static_cast<int>(atof(state.c_str()));
    if (percentage_step != 0) {
      if (can_update_from_api()) {
        // prevent dividing by 0, store percentage until percentage_step is loaded
        int new_speed = percentage / percentage_step;
        ESP_LOGI(TAG,
                 "percentage_changed: updating '%s' to %d, step is '%d', speed "
                 "is '%d'",
                 this->get_name().c_str(), percentage, percentage_step,
                 new_speed);
        auto call = this->make_call();
        call.set_speed(new_speed);
        call.perform();
      }
    }
    next_api_publish_ = true;
  }

  void percentage_step_changed(std::string state) {
    next_api_publish_ = false;
    ESP_LOGI(TAG, "percentage_step_changed: '%s' to %s",
             this->get_name().c_str(), state.c_str());
    if (can_update_from_api()) {
      percentage_step = static_cast<int>(atof(state.c_str()));
      if (percentage != 0) {
        // use percentage to calculate speed
        int new_speed = percentage / percentage_step;
        ESP_LOGI(TAG,
                 "percentage_step_changed: updating '%s' to %d, step is '%d', "
                 "speed is '%d'",
                 this->get_name().c_str(), percentage, percentage_step,
                 new_speed);
        auto call = this->make_call();
        call.set_speed(new_speed);
        call.perform();
      } else {
        ESP_LOGI(TAG,
                 "percentage_step_changed: not updating '%s' to %d, step is "
                 "'%d' and speed is '%d'",
                 this->get_name().c_str(), percentage, percentage_step, speed);
      }
    }
    next_api_publish_ = true;
  }

  void control(const fan::FanCall& call) {
    if (call.get_state().has_value()) {
      ESP_LOGI(TAG,
               "control: '%s', can update: %d, setting state new: %d old: %d",
               this->get_name().c_str(), can_update_from_api(),
               *call.get_state(), this->state);
      this->state = *call.get_state();
    }
    if (call.get_speed().has_value() && *call.get_speed() != this->speed) {
      ESP_LOGI(TAG,
               "control: '%s', can update: %d, setting speed new: %d old: %d",
               this->get_name().c_str(), can_update_from_api(),
               *call.get_speed(), this->speed);
      this->speed = *call.get_speed();
    }
    this->publish_state();
    if (next_api_publish_) {
      publish_api_state(call);
    }
    this->state_callback_.call();
  }

  fan::FanTraits get_traits() {
    auto traits = fan::FanTraits();
    // traits.set_oscillation(true);
    traits.set_speed(true);
    if (percentage_step != 0) {
      traits.set_supported_speed_count(100 / percentage_step);
    } else {
      traits.set_supported_speed_count(1);
    }
    // traits.set_direction(true);
    return traits;
  }
};
}  // namespace homeassistant_fan
}  // namespace esphome
