#pragma once

#include <memory>
#include <string>
#include <vector>
#include "HomeAssistantBaseMediaPlayer.h"

namespace esphome {
namespace homeassistant_media_player {

enum MediaPlayerTVRemoteCommand {
  MEDIA_PLAYER_TV_COMMAND_UP,
  MEDIA_PLAYER_TV_COMMAND_DOWN,
  MEDIA_PLAYER_TV_COMMAND_LEFT,
  MEDIA_PLAYER_TV_COMMAND_RIGHT,
  MEDIA_PLAYER_TV_COMMAND_SELECT,
  MEDIA_PLAYER_TV_COMMAND_BACK,
  MEDIA_PLAYER_TV_COMMAND_HOME,
  MEDIA_PLAYER_TV_COMMAND_POWER,
  MEDIA_PLAYER_TV_COMMAND_VOLUME_UP,
  MEDIA_PLAYER_TV_COMMAND_VOLUME_DOWN,
  MEDIA_PLAYER_TV_COMMAND_PAUSE
};

class HomeAssistantTVMediaPlayer : public HomeAssistantBaseMediaPlayer {
 public:
  void setup() override;

  media_player::MediaPlayerTraits get_traits() {
    auto traits = media_player::MediaPlayerTraits();
    traits.set_supports_pause(true);
    return traits;
  }

  virtual void tvRemoteCommand(MediaPlayerTVRemoteCommand command);
  void increaseVolume() override;
  void decreaseVolume() override;
  RemotePlayerType get_player_type() { return TVRemotePlayerType; }
  void player_source_changed(std::string state);
  void set_soundbar(HomeAssistantBaseMediaPlayer* new_soundbar) {
    soundbar_ = new_soundbar;
  }
  HomeAssistantBaseMediaPlayer* get_soundbar() { return soundbar_; }

  HomeAssistantTVMediaPlayer& set_command(MediaPlayerTVRemoteCommand command);
  HomeAssistantTVMediaPlayer& set_command(
      optional<MediaPlayerTVRemoteCommand> command);
  HomeAssistantTVMediaPlayer& set_command(const std::string& command);

 protected:
  HomeAssistantBaseMediaPlayer* soundbar_{nullptr};
  void subscribe_source() override;
  void sources_changed(std::string state) override;
  void subscribe_sources() override;
  media_player_source::MediaPlayerSourceInternal* device_sources{nullptr};
  virtual std::string stringForRemoteCommand(
      MediaPlayerTVRemoteCommand command);
};
}  // namespace homeassistant_media_player
}  // namespace esphome
