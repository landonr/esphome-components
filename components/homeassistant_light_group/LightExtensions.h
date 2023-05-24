#pragma once

#include <string>
#include "esphome/components/light/light_output.h"
#include "esphome/components/light/light_state.h"

namespace esphome {
namespace light {
static bool supportsBrightness(light::LightState* light) {
  return light->get_traits().supports_color_capability(
      light::ColorCapability::BRIGHTNESS);
}
static bool supportsColorTemperature(light::LightState* light) {
  return light->get_traits().supports_color_capability(
      light::ColorCapability::COLOR_TEMPERATURE);
}
static bool supportsColor(light::LightState* light) {
  return light->get_traits().supports_color_capability(
      light::ColorCapability::RGB);
}

static int get_hsv_color(light::LightState* light) {
  auto red = light->remote_values.get_red();
  auto green = light->remote_values.get_green();
  auto blue = light->remote_values.get_blue();
  int hue = 0;
  float saturation, value = 0;
  rgb_to_hsv(red, green, blue, hue, saturation, value);
  return hue;
}

static Color rgbLightColor(light::LightState* light) {
  if (!supportsBrightness(light)) {
    return Color(255, 255, 255);
  }
  auto red = light->remote_values.get_red() * 255;
  auto green = light->remote_values.get_green() * 255;
  auto blue = light->remote_values.get_blue() * 255;
  return Color(red, green, blue);
}

static std::string icon(light::LightState* light) {
  if (light->remote_values.is_on()) {
    return "󰌵";
  } else {
    return "󰌶";
  }
}
}  // namespace light
}  // namespace esphome
