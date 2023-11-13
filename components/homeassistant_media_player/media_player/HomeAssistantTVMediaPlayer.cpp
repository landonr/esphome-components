#include "HomeAssistantTVMediaPlayer.h"
#include "JSONTextHelpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace homeassistant_media_player {

static const char* const TAG = "homeassistant.media_player_tv";

void HomeAssistantTVMediaPlayer::setup() {
  HomeAssistantBaseMediaPlayer::setup();
}

void HomeAssistantTVMediaPlayer::subscribe_source() {
  ESP_LOGI(TAG, "subscribe_source: %s", this->entity_id_.c_str());
  device_sources = new media_player_source::MediaPlayerSourceInternal();
  device_sources->set_name("Inputs");
  register_source(device_sources);
  subscribe_homeassistant_state(
      &HomeAssistantTVMediaPlayer::player_source_changed, this->entity_id_,
      "source");
}

void HomeAssistantTVMediaPlayer::player_source_changed(std::string state) {
  ESP_LOGI(TAG, "player_source_changed: %s changed to %s", get_name().c_str(),
           state.c_str());
  if (state.find("YouTube") != std::string::npos) {
    mediaSource = YouTubeRemotePlayerMediaSource;
  } else if (state.find("Netflix") != std::string::npos) {
    mediaSource = NetflixRemotePlayerMediaSource;
  } else if (state.find("Plex") != std::string::npos) {
    mediaSource = PlexRemotePlayerMediaSource;
  } else if (state.find("Roku") != std::string::npos ||
             state.find("idle") != std::string::npos) {
    mediaSource = HomeRemotePlayerMediaSource;
    mediaTitle = "";
  } else if (state.find("tvshow") != std::string::npos ||
             state.find("playing") != std::string::npos) {
    mediaSource = TVRemotePlayerMediaSource;
  } else {
    mediaSource = NoRemotePlayerMediaSource;
  }
}

void HomeAssistantTVMediaPlayer::subscribe_sources() {
  ESP_LOGI(TAG, "subscribe_sources: %s", this->entity_id_.c_str());
  subscribe_homeassistant_state(&HomeAssistantTVMediaPlayer::sources_changed,
                                this->entity_id_, "source_list");
}

void HomeAssistantTVMediaPlayer::sources_changed(std::string state) {
  ESP_LOGI(TAG, "sources_changed: %s - %s", get_name().c_str(), state.c_str());
  device_sources->set_sources_json_array(replaceAll(state, "\\xa0", " "));
}

void HomeAssistantTVMediaPlayer::increaseVolume() {
  if (get_soundbar() != nullptr) {
    get_soundbar()->increaseVolume();
    return;
  }
  tvRemoteCommand(VOLUME_UP_COMMAND);
}

void HomeAssistantTVMediaPlayer::decreaseVolume() {
  if (get_soundbar() != nullptr) {
    get_soundbar()->decreaseVolume();
    return;
  }
  tvRemoteCommand(VOLUME_DOWN_COMMAND);
}

void HomeAssistantTVMediaPlayer::tvRemoteCommand(
    MediaPlayerTVRemoteCommand command) {
  std::string remoteName = entity_id_.substr(12).insert(0, "remote");
  auto commandString = stringForRemoteCommand(command);
  ESP_LOGI(TAG, "tvRemoteCommand: %s, %s", commandString.c_str(),
           remoteName.c_str());
  call_homeassistant_service("remote.send_command",
                             {
                                 {"entity_id", remoteName},
                                 {"command", commandString.c_str()},
                             });
}

}  // namespace homeassistant_media_player
}  // namespace esphome
