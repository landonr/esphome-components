#pragma once

#include <memory>
#include <vector>

#ifdef USE_API_LIGHT
#include "esphome/components/homeassistant_light_group/light/HomeAssistantLight.h"
#endif

#include "esphome/components/light/light_state.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/color.h"
#include "esphome/core/component.h"

namespace esphome {
namespace homeassistant_light_group {

class HomeAssistantLightGroup : public Component, public sensor::Sensor {
 public:
  std::vector<light::LightState*> lights;
  bool selectLightDetailAtIndex(int index);
  bool selectLightDetail(light::LightState* new_active_light);
  void clearActiveLight() { _activeLight = NULL; }
  light::LightState* getActiveLight() { return _activeLight; }
  bool lightDetailSelected = false;
  void register_light(light::LightState* newLight);
  void toggleLight(int index);
  float get_setup_priority() const override { return setup_priority::LATE; }
  void update_color_with_hsv(const float hsv_color, light::LightState* light);

  void decTemperature();
  void incTemperature();
  void decBrightness();
  void incBrightness();
  void toggle();
  void decColor();
  void incColor();

 private:
  light::LightState* _activeLight = NULL;
};

}  // namespace homeassistant_light_group
}  // namespace esphome
