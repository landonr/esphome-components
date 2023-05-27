#include "HomeAssistantTVAndroidMediaPlayer.h"
#include "JSONTextHelpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace homeassistant_media_player {

static const char* const TAG = "homeassistant.media_player_android";

void HomeAssistantTVAndroidMediaPlayer::setup() {
  supported_features_.push_back(
      std::make_shared<MediaPlayerSupportedFeature>(TV_BACK));
  supported_features_.push_back(
      std::make_shared<MediaPlayerSupportedFeature>(TV_HOME));
  supported_features_.push_back(
      std::make_shared<MediaPlayerSupportedFeature>(MENU_HOME));
  supported_features_.push_back(
      std::make_shared<MediaPlayerSupportedFeature>(REMOTE_MODE));
  HomeAssistantTVMediaPlayer::setup();
}

std::string HomeAssistantTVAndroidMediaPlayer::stringForRemoteCommand(
    MediaPlayerTVRemoteCommand command) {
  switch (command) {
    case UP:
      return "DPAD_UP";
    case DOWN:
      return "DPAD_DOWN";
    case LEFT:
      return "DPAD_LEFT";
    case RIGHT:
      return "DPAD_RIGHT";
    case SELECT:
      return "DPAD_CENTER";
    case BACK:
      return "BACK";
    case HOME:
      return "HOME";
    case POWER:
      return "power";
    case VOLUME_UP:
      return "VOLUME_UP";
    case VOLUME_DOWN:
      return "VOLUME_DOWN";
    case PAUSE_COMMAND:
      return "MEDIA_PLAY_PAUSE";
  }
  return "";
}

}  // namespace homeassistant_media_player
}  // namespace esphome
