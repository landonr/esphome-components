#include "HomeAssistantLightGroup.h"
#include "LightExtensions.h"
#include "esphome/core/log.h"

namespace esphome {
namespace homeassistant_light_group {

static const char* const TAG = "homeassistant.light.group";

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

}  // namespace homeassistant_light_group
}  // namespace esphome
