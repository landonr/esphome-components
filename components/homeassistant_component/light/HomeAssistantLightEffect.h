#pragma once
#include "esphome/components/light/light_effect.h"
#include "esphome/components/light/addressable_light_effect.h"

namespace esphome {
namespace homeassistant_light {

class HomeAssistantLightEffect : public light::AddressableLightEffect {
 public:
  explicit HomeAssistantLightEffect(const std::string &name) : light::AddressableLightEffect(name) {}
  void start_internal() override;
  void stop() override;
  void apply() override;
};

}  // namespace homeassistant_light
}  // namespace esphome
