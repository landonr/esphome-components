#pragma once

#include <utility>
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/automation.h"
#include "esphome/core/component.h"

// based on https://github.com/m5stack/M5Unit-UnitEncoderC/blob/main/src/Unit_MiniEncoderC.cpp

namespace esphome {
namespace unitencoderc {

class UnitEncoderC : public i2c::I2CDevice, public Component {
 public:
  void setup() override;
  void loop() override;

  void set_number(uint8_t number) { number_ = number; }
  void set_min_value(int32_t min_value) { this->min_value_ = min_value; }
  void set_max_value(int32_t max_value) { this->max_value_ = max_value; }

  float get_setup_priority() const override;

  void add_on_clockwise_callback(std::function<void()> callback) {
    this->on_clockwise_callback_.add(std::move(callback));
  }

  void add_on_anticlockwise_callback(std::function<void()> callback) {
    this->on_anticlockwise_callback_.add(std::move(callback));
  }

  void set_encoder(sensor::Sensor* encoder) { this->encoder_value_ = encoder; }

  void set_button(binary_sensor::BinarySensor* button) {
    this->button_ = button;
  }

  void set_encoder_filter(int encoder_filter) {
    this->encoder_filter_ = encoder_filter;
  }

  // void set_light(MiniEncoderCLightOutput* light) { this->light_ = light; }

 protected:
  uint8_t number_{0};
  int32_t value_{0};
  int encoder_filter_ = 1;
  int32_t min_value_{INT32_MIN};
  int32_t max_value_{INT32_MAX};

  CallbackManager<void()> on_clockwise_callback_;
  CallbackManager<void()> on_anticlockwise_callback_;
  sensor::Sensor* encoder_value_{nullptr};
  sensor::Sensor* increment_value_{nullptr};
  sensor::Sensor* firmware_version_{nullptr};
  binary_sensor::BinarySensor* button_{nullptr};
  // MiniEncoderCLightOutput* light_{nullptr};
  void setEncoderValue(int32_t value);
};

class UnitEncoderCClockwiseTrigger : public Trigger<> {
 public:
  explicit UnitEncoderCClockwiseTrigger(UnitEncoderC* parent) {
    parent->add_on_clockwise_callback([this]() { this->trigger(); });
  }
};

class UnitEncoderCAnticlockwiseTrigger : public Trigger<> {
 public:
  explicit UnitEncoderCAnticlockwiseTrigger(UnitEncoderC* parent) {
    parent->add_on_anticlockwise_callback([this]() { this->trigger(); });
  }
};

}  // namespace unitencoderc
}  // namespace esphome
