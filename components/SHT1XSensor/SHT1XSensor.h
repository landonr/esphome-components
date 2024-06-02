#pragma once

#include "SHT1x-ESP.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace sht1xsensor {

/// This class implements support for
class SHT1XSensor : public sensor::Sensor, public PollingComponent {
 public:
  void setup() override;
  void update() override;
  void set_data(int pin) { data = pin; }
  void set_clock(int pin) { clock = pin; }
  void set_temperature_sensor(Sensor* sensor) { temperature_sensor = sensor; }
  Sensor* get_temperature_sensor() { return temperature_sensor; }
  void set_humidity_sensor(Sensor* sensor) { humidity_sensor = sensor; }
  Sensor* get_humidity_sensor() { return humidity_sensor; }

 protected:
  int data;
  int clock;

  SHT1x* sht1x_;
  Sensor* temperature_sensor;
  Sensor* humidity_sensor;
};

}  // namespace sht1xsensor
}  // namespace esphome