#pragma once

#include <memory>
#include <string>
#include <vector>
#include "HomeAssistantTVMediaPlayer.h"

namespace esphome {
namespace homeassistant_media_player {

class HomeAssistantTVRokuMediaPlayer : public HomeAssistantTVMediaPlayer {
 public:
  void setup() override;

 protected:
  std::string stringForRemoteCommand(
      MediaPlayerTVRemoteCommand command) override;
  void playMedia(media_player_source::MediaPlayerSourceItem* source) override;

 private:
  std::string idForSourceApp(media_player_source::AppPlayerSourceType source);
};
}  // namespace homeassistant_media_player
}  // namespace esphome
