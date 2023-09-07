#pragma once

#include <vector>
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/light/light_output.h"
#include "esphome/components/light/light_state.h"
#include "esphome/components/light/light_traits.h"
#include "esphome/core/color.h"
#include "esphome/core/component.h"
#include "esphome/core/log.h"

// based on https://github.com/m5stack/M5Unit-MiniEncoderC/blob/main/src/Unit_MiniEncoderC.cpp

namespace esphome {
namespace miniencoderc {
static const char* const LIGHT_TAG = "mini_encoder_c_light";
static const uint8_t MINI_ENCODER_C_LIGHT = 0x30;

class MiniEncoderCLightOutput : public Component,
                                public light::LightOutput,
                                public i2c::I2CDevice {
 public:
  float get_setup_priority() const { return setup_priority::IO; }

  void setup() override {
    uint8_t data[4];
    data[2] = static_cast<uint8_t>(255.0f);
    data[1] = static_cast<uint8_t>(255.0f);
    data[0] = static_cast<uint8_t>(255.0f);

    if (this->write_register(MINI_ENCODER_C_LIGHT, data, 3) != i2c::ERROR_OK) {
      ESP_LOGE(LIGHT_TAG, "MiniEncoderC light set failed");
      this->mark_failed();
    }
  }

  light::LightTraits get_traits() override {
    // return the traits this light supports
    auto traits = light::LightTraits();
    traits.set_supported_color_modes(
        {light::ColorMode::RGB, light::ColorMode::BRIGHTNESS});
    return traits;
  }

  void write_state(light::LightState* state) override {
    // This will be called by the light to get a new state to be written.
    float red, green, blue;
    // use any of the provided current_values methods
    state->current_values_as_rgb(&red, &green, &blue);
    // Write red, green and blue to HW
    uint8_t data[4];
    data[2] = static_cast<uint8_t>(blue * 255.0f);
    data[1] = static_cast<uint8_t>(green * 255.0f);
    data[0] = static_cast<uint8_t>(red * 255.0f);

    if (this->write_register(MINI_ENCODER_C_LIGHT, data, 3) != i2c::ERROR_OK) {
      ESP_LOGE(LIGHT_TAG, "MiniEncoderC light set failed");
      this->mark_failed();
    }
  }
};

}  // namespace miniencoderc
}  // namespace esphome
