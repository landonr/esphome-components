#include "HomeAssistantLightEffect.h"
#include "esphome/core/log.h"

namespace esphome {
namespace homeassistant_light {

static const char* const TAG = "homeassistant.lighteffect";

void HomeAssistantLightEffect::start_internal() {
  ESP_LOGI(TAG, "'%s': start_internal changed",
           get_name().c_str());
}

void HomeAssistantLightEffect::stop() {
  ESP_LOGI(TAG, "'%s': stop changed",
           get_name().c_str());

}

void HomeAssistantLightEffect::apply() {
  ESP_LOGI(TAG, "'%s': apply changed",
           get_name().c_str());

}
}  // namespace homeassistant_light
}  // namespace esphome
