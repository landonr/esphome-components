#pragma once

#include <memory>
#include <string>
#include <vector>
#include "HomeAssistantTVMediaPlayer.h"

namespace esphome {
namespace homeassistant_media_player {

class HomeAssistantTVAndroidMediaPlayer : public HomeAssistantTVMediaPlayer {
 public:
  void setup() override;

 protected:
  std::string stringForRemoteCommand(
      MediaPlayerTVRemoteCommand command) override;
};
}  // namespace homeassistant_media_player
}  // namespace esphome
