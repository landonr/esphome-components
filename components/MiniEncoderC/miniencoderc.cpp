#include "miniencoderc.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace miniencoderc {

static const char* const TAG = "mini_encoder_c";

static const uint8_t MINI_ENCODER_C_FIRMWARE = 0xFE;
static const uint8_t MINI_ENCODER_C_RESET_COUNTER = 0x40;
static const uint8_t MINI_ENCODER_C_ENCODER = 0x00;
static const uint8_t MINI_ENCODER_C_INCREMENT = 0x10;
static const uint8_t MINI_ENCODER_C_BUTTON = 0x20;

float MiniEncoderC::get_setup_priority() const {
  return setup_priority::IO;
}

void MiniEncoderC::setup() {
  ESP_LOGI(TAG, "Setting up MiniEncoderC...");
  uint8_t firmware;
  if (this->read_register(MINI_ENCODER_C_FIRMWARE, &firmware, 1) !=
      i2c::ERROR_OK) {
    ESP_LOGE(TAG, "MiniEncoderC Setup Failed");
    this->mark_failed();
    return;
  }
  setEncoderValue(0);
  ESP_LOGI(TAG, "MiniEncoderC Firmware: %d", firmware);
}

void MiniEncoderC::setEncoderValue(int32_t value) {
  uint8_t data[4];

  data[0] = value & 0xff;
  data[1] = (value >> 8) & 0xff;
  data[2] = (value >> 16) & 0xff;
  data[3] = (value >> 24) & 0xff;

  if (this->write_register(MINI_ENCODER_C_ENCODER, data, 4) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "MiniEncoderC encoder zero setup failed");
    this->mark_failed();
  }
}

void MiniEncoderC::loop() {
  uint8_t data[4];
  if (this->read_register(MINI_ENCODER_C_ENCODER, data, 4) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "unable to read encoder level");
    this->mark_failed();
    return;
  }
  int32_t value = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);

  int filtered_value = static_cast<int>(value) / this->encoder_filter_;
  if (this->encoder_value_ != nullptr) {
    if (!this->encoder_value_->has_state() ||
        (this->encoder_value_->state != filtered_value)) {
      ESP_LOGD(TAG, "MiniEncoderC value: %d filtered = %d", value,
               filtered_value);
      if (this->encoder_value_->state < filtered_value) {
        ESP_LOGD(TAG, "MiniEncoderC clockwise value: %d", filtered_value);
        this->encoder_value_->publish_state(filtered_value);
        this->on_clockwise_callback_.call();
      } else {
        ESP_LOGD(TAG, "MiniEncoderC anticlockwise value: %d", filtered_value);
        this->encoder_value_->publish_state(filtered_value);
        this->on_anticlockwise_callback_.call();
      }
    }
  }

  if (this->read_register(MINI_ENCODER_C_BUTTON, data, 1) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "unable to read button level");
    this->mark_failed();
    return;
  }
  bool button_state = data[0] == 0x01;
  if (this->button_ != nullptr) {
    if (!this->button_->has_state() || (this->button_->state != button_state)) {
      ESP_LOGD(TAG, "MiniEncoderC button: %d", button_state);
      this->button_->publish_state(button_state);
    }
  }
}

}  // namespace miniencoderc
}  // namespace esphome
