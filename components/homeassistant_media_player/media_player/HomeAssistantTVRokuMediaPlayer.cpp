#include "HomeAssistantTVRokuMediaPlayer.h"
#include "JSONTextHelpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace homeassistant_media_player {

static const char* const TAG = "homeassistant.media_player_roku";

void HomeAssistantTVRokuMediaPlayer::setup() {
  supported_features_.push_back(TV_BACK);
  supported_features_.push_back(TV_HOME);
  supported_features_.push_back(MENU_HOME);
  supported_features_.push_back(REMOTE_MODE);
  HomeAssistantTVMediaPlayer::setup();
}

std::string HomeAssistantTVRokuMediaPlayer::stringForRemoteCommand(
    MediaPlayerTVRemoteCommand command) {
  switch (command) {
    case MEDIA_PLAYER_TV_COMMAND_UP:
      return "up";
    case MEDIA_PLAYER_TV_COMMAND_DOWN:
      return "down";
    case MEDIA_PLAYER_TV_COMMAND_LEFT:
      return "left";
    case MEDIA_PLAYER_TV_COMMAND_RIGHT:
      return "right";
    case MEDIA_PLAYER_TV_COMMAND_SELECT:
      return "select";
    case MEDIA_PLAYER_TV_COMMAND_BACK:
      return "back";
    case MEDIA_PLAYER_TV_COMMAND_HOME:
      return "home";
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

std::string HomeAssistantTVRokuMediaPlayer::idForSourceApp(
    media_player_source::AppPlayerSourceType source) {
  switch (source) {
    case media_player_source::AppPlayerSourceType::AppPlayerSourceTypeNone:
      return "none";
    case media_player_source::AppPlayerSourceType::AppPlayerSourceTypeYouTube:
      return "837";
    case media_player_source::AppPlayerSourceType::AppPlayerSourceTypeSpotify:
      return "22297";
    case media_player_source::AppPlayerSourceType::AppPlayerSourceTypeNetflix:
      return "netflix";
    case media_player_source::AppPlayerSourceType::AppPlayerSourceTypePlex:
      return "plex";
    case media_player_source::AppPlayerSourceType::
        AppPlayerSourceTypeDisneyPlus:
      return "291097";
    case media_player_source::AppPlayerSourceType::AppPlayerSourceTypeHulu:
      return "2285";
    case media_player_source::AppPlayerSourceType::
        AppPlayerSourceTypePrimeVideo:
      return "prime_video";
    case media_player_source::AppPlayerSourceType::AppPlayerSourceTypeHBO:
      return "hbo";
  }
  return "";
}

void HomeAssistantTVRokuMediaPlayer::playMedia(
    media_player_source::MediaPlayerSourceItem* source) {
  ESP_LOGI(TAG, "playMedia: %s", this->entity_id_.c_str());
  switch (source->get_media_type()) {
    case media_player_source::MediaPlayerSourceType::MediaPlayerSourceTypeApp: {
      ESP_LOGI(TAG,
               "playMedia: %s content id %s source type %s media type %d "
               "source app %s",
               this->entity_id_.c_str(), source->get_media_content_id().c_str(),
               source->sourceTypeString().c_str(), source->get_media_type(),
               idForSourceApp(source->get_app_type()).c_str());

      call_homeassistant_service(
          "script.homething_roku_play_media",
          {
              {"entity_id", this->entity_id_.c_str()},
              {"media_content_id",
               idForSourceApp(source->get_app_type()).c_str()},
              {"media_content_type", source->sourceTypeString().c_str()},
              {"content_id", source->get_media_content_id().c_str()},
              {"media_type", "live"},
          });
      return;
    }
    default:
      break;
  }
  HomeAssistantBaseMediaPlayer::playMedia(source);
}

}  // namespace homeassistant_media_player
}  // namespace esphome
