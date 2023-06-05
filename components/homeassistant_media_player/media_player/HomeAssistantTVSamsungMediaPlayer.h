#pragma once

#include <memory>
#include <string>
#include <vector>
#include "HomeAssistantTVMediaPlayer.h"

namespace esphome {
namespace homeassistant_media_player {

class HomeAssistantTVSamsungMediaPlayer : public HomeAssistantTVMediaPlayer {
 public:
  void setup() override;
  void tvRemoteCommand(MediaPlayerTVRemoteCommand command) override;

 private:
  void subscribe_media_artist() override;

 protected:
  std::string stringForRemoteCommand(MediaPlayerTVRemoteCommand command) override;
};
}  // namespace homeassistant_media_player
}  // namespace esphome
