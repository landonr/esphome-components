#include "HomeAssistantTVKodiMediaPlayer.h"
#include "JSONTextHelpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace homeassistant_media_player {

static const char* const TAG = "homeassistant.media_player_kodi";

void HomeAssistantTVKodiMediaPlayer::setup() {
  supported_features_.push_back(TV_BACK);
  supported_features_.push_back(TV_HOME);
  supported_features_.push_back(MENU_HOME);
  supported_features_.push_back(REMOTE_MODE);
  HomeAssistantTVMediaPlayer::setup();
}

void HomeAssistantTVKodiMediaPlayer::subscribe_source() {
  ESP_LOGI(TAG, "subscribe_source: %s", this->entity_id_.c_str());
  subscribe_homeassistant_state(
      &HomeAssistantTVMediaPlayer::player_source_changed, this->entity_id_,
      "media_content_type");
}

void HomeAssistantTVKodiMediaPlayer::subscribe_media_artist() {
  ESP_LOGI(TAG, "subscribe_media_artist: %s", this->entity_id_.c_str());
  subscribe_homeassistant_state(
      &HomeAssistantTVMediaPlayer::media_artist_changed, this->entity_id_,
      "media_series_title");
}

std::string HomeAssistantTVKodiMediaPlayer::stringForRemoteCommand(
    MediaPlayerTVRemoteCommand command) {
  switch (command) {
    case MEDIA_PLAYER_TV_COMMAND_UP:
      return "Input.Up";
    case MEDIA_PLAYER_TV_COMMAND_DOWN:
      return "Input.Down";
    case MEDIA_PLAYER_TV_COMMAND_LEFT:
      return "Input.Left";
    case MEDIA_PLAYER_TV_COMMAND_RIGHT:
      return "Input.Right";
    case MEDIA_PLAYER_TV_COMMAND_SELECT:
      return "Input.Select";
    case MEDIA_PLAYER_TV_COMMAND_BACK:
      return "Input.Back";
    case MEDIA_PLAYER_TV_COMMAND_HOME:
      return "Input.ContextMenu";
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

void HomeAssistantTVKodiMediaPlayer::tvRemoteCommand(
    MediaPlayerTVRemoteCommand command) {
  auto commandString = stringForRemoteCommand(command);
  ESP_LOGI(TAG, "tvRemoteCommand: %s, %s", commandString.c_str(),
           entity_id_.c_str());
  call_homeassistant_service(
      "kodi.call_method",
      {{"entity_id", entity_id_.c_str()}, {"method", commandString.c_str()}});
}
}  // namespace homeassistant_media_player
}  // namespace esphome
