#include "HomeAssistantTVAndroidMediaPlayer.h"
#include "esphome/core/log.h"

namespace esphome {
namespace homeassistant_media_player {

static const char* const TAG = "homeassistant.media_player_android";

void HomeAssistantTVAndroidMediaPlayer::setup() {
  supported_features_.push_back(TV_BACK);
  supported_features_.push_back(TV_HOME);
  supported_features_.push_back(MENU_HOME);
  supported_features_.push_back(REMOTE_MODE);
  HomeAssistantTVMediaPlayer::setup();
}

std::string HomeAssistantTVAndroidMediaPlayer::stringForRemoteCommand(
    MediaPlayerTVRemoteCommand command) {
  switch (command) {
    case MEDIA_PLAYER_TV_COMMAND_UP:
      return "DPAD_UP";
    case MEDIA_PLAYER_TV_COMMAND_DOWN:
      return "DPAD_DOWN";
    case MEDIA_PLAYER_TV_COMMAND_LEFT:
      return "DPAD_LEFT";
    case MEDIA_PLAYER_TV_COMMAND_RIGHT:
      return "DPAD_RIGHT";
    case MEDIA_PLAYER_TV_COMMAND_SELECT:
      return "DPAD_CENTER";
    case MEDIA_PLAYER_TV_COMMAND_BACK:
      return "BACK";
    case MEDIA_PLAYER_TV_COMMAND_HOME:
      return "HOME";
    case MEDIA_PLAYER_TV_COMMAND_POWER:
      return "power";
    case MEDIA_PLAYER_TV_COMMAND_VOLUME_UP:
      return "volume_up";
    case MEDIA_PLAYER_TV_COMMAND_VOLUME_DOWN:
      return "volume_down";
    case MEDIA_PLAYER_TV_COMMAND_PAUSE:
      return "pause";
  }
  return "";
}

}  // namespace homeassistant_media_player
}  // namespace esphome
