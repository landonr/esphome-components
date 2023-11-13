#include "HomeAssistantTVSamsungMediaPlayer.h"
#include "JSONTextHelpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace homeassistant_media_player {

static const char* const TAG = "homeassistant.media_player_samsung";

void HomeAssistantTVSamsungMediaPlayer::setup() {
  supported_features_.push_back(TV_BACK);
  supported_features_.push_back(TV_HOME);
  supported_features_.push_back(MENU_HOME);
  supported_features_.push_back(REMOTE_MODE);
  HomeAssistantTVMediaPlayer::setup();
}

void HomeAssistantTVSamsungMediaPlayer::subscribe_media_artist() {
  ESP_LOGI(TAG, "subscribe_media_artist: %s", this->entity_id_.c_str());
  subscribe_homeassistant_state(
      &HomeAssistantBaseMediaPlayer::media_artist_changed, this->entity_id_,
      "media_series_title");
}

std::string HomeAssistantTVSamsungMediaPlayer::stringForRemoteCommand(
    MediaPlayerTVRemoteCommand command) {
  switch (command) {
    case MEDIA_PLAYER_TV_COMMAND_UP:
      return "KEY_UP";
    case MEDIA_PLAYER_TV_COMMAND_DOWN:
      return "KEY_DOWN";
    case MEDIA_PLAYER_TV_COMMAND_LEFT:
      return "KEY_LEFT";
    case MEDIA_PLAYER_TV_COMMAND_RIGHT:
      return "KEY_RIGHT";
    case MEDIA_PLAYER_TV_COMMAND_SELECT:
      return "KEY_ENTER";
    case MEDIA_PLAYER_TV_COMMAND_BACK:
      return "KEY_RETURN";
    case MEDIA_PLAYER_TV_COMMAND_HOME:
      return "KEY_HOME";
    case MEDIA_PLAYER_TV_COMMAND_POWER:
      return "KEY_CHDOWN";
    case MEDIA_PLAYER_TV_COMMAND_VOLUME_UP:
      return "KEY_VOLUP";
    case MEDIA_PLAYER_TV_COMMAND_VOLUME_DOWN:
      return "KEY_VOLDOWN";
    case MEDIA_PLAYER_TV_COMMAND_PAUSE:
      return "KEY_PAUSE";
  }
  return "";
}

void HomeAssistantTVSamsungMediaPlayer::tvRemoteCommand(
    MediaPlayerTVRemoteCommand command) {
  auto commandString = stringForRemoteCommand(command);
  ESP_LOGI(TAG, "tvRemoteCommand: %s, %s", commandString.c_str(),
           entity_id_.c_str());
  call_homeassistant_service("media_player.play_media",
                             {{"entity_id", entity_id_.c_str()},
                              {"media_content_type", "send_key"},
                              {"media_content_id", commandString.c_str()}});
}
}  // namespace homeassistant_media_player
}  // namespace esphome
