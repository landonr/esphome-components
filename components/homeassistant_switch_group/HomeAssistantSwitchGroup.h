#pragma once
#include <memory>
#include <vector>

#include "esphome/components/homeassistant_switch_group/switch/HomeAssistantSwitch.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/switch/switch.h"
#include "esphome/core/component.h"

namespace esphome {
namespace homeassistant_switch_group {
class HomeAssistantSwitchGroup : public Component, public sensor::Sensor {
 public:
  std::vector<switch_::Switch*> switches;
  float get_setup_priority() const override { return setup_priority::LATE; }
  bool selectSwitch(int index);
  void register_switch(switch_::Switch* newSwitch);
};

}  // namespace homeassistant_switch_group
}  // namespace esphome
