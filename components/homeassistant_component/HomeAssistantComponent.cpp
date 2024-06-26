#include "HomeAssistantComponent.h"
#include "esphome/core/log.h"

namespace esphome {
namespace homeassistant_component {

static const char* const TAG = "homeassistant.component";

bool HomeAssistantComponent::can_update_from_api() {
  if (ignore_update_until_ == 0) {
    ESP_LOGD(TAG, "can_update_from_api: true %s %d", get_entity_id().c_str(),
             ignore_update_until_);
    return true;
  } else {
    ESP_LOGD(TAG, "can_update_from_api: %s %d", get_entity_id().c_str(),
             micros() > ignore_update_until_);
    return micros() > ignore_update_until_;
  }
}

void HomeAssistantComponent::ignore_api_updates_with_seconds(double seconds) {
  ESP_LOGD(TAG, "ignore_api_updates_with_seconds: %s %f",
           get_entity_id().c_str(), seconds);
  uint32_t delayTime = uint32_t(seconds * 1000000.0);
  ignore_update_until_ = micros() + delayTime;
}

bool HomeAssistantComponent::can_publish_to_api() {
  if (ignore_publish_until_ == 0) {
    ESP_LOGD(TAG, "can_publish_to_api: true %s %d", get_entity_id().c_str(),
             ignore_publish_until_);
    return true;
  } else {
    ESP_LOGD(TAG, "can_publish_to_api: %s %d", get_entity_id().c_str(),
             micros() > ignore_publish_until_);
    return micros() > ignore_publish_until_;
  }
}

void HomeAssistantComponent::ignore_publish_with_seconds(double seconds) {
  ESP_LOGD(TAG, "ignore_publish_with_seconds: %s %f", get_entity_id().c_str(),
           seconds);
  uint32_t delayTime = uint32_t(seconds * 1000000.0);
  ignore_publish_until_ = micros() + delayTime;
}

}  // namespace homeassistant_component
}  // namespace esphome
