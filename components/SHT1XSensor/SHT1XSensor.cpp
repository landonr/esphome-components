#include "SHT1XSensor.h"
#include "esphome/core/log.h"
namespace esphome {
namespace sht1xsensor {

#define MIN_TEMP -40
#define MAX_TEMP 254

static const char* TAG = "sht1xsensor.sensor";

void SHT1XSensor::setup() {
  ESP_LOGCONFIG(TAG, "Setting up SHT1X '%s'...", this->name_.c_str());
  ESP_LOGCONFIG(TAG, "Data pin is '%d'...", this->data);
  ESP_LOGCONFIG(TAG, "Clock pin is '%d'...", this->clock);
  this->sht1x_ = new SHT1x(this->data, this->clock);
  ESP_LOGCONFIG(TAG, "Done setting up SHT1X '%s'...", this->name_.c_str());
}

void SHT1XSensor::update() {
  float temp = this->sht1x_->readTemperatureC();
  if (temp >= MIN_TEMP && temp <= MAX_TEMP) {
    this->temperature_sensor->publish_state(temp);
  }
  float humidity = this->sht1x_->readHumidity();
  if (humidity >= 0 && humidity <= 100) {
    this->humidity_sensor->publish_state(humidity);
  }
}
}  // namespace sht1xsensor
}  // namespace esphome