#include "HomeAssistantSpeakerMediaPlayer.h"
#include "esphome/core/log.h"

namespace esphome {
namespace homeassistant_media_player {

static const char* const TAG = "homeassistant.media_player_speaker";

void HomeAssistantSpeakerMediaPlayer::setup() {
  HomeAssistantBaseMediaPlayer::setup();
}

void HomeAssistantSpeakerMediaPlayer::subscribe_source() {
  ESP_LOGI(TAG, "subscribe_source: %s", this->entity_id_.c_str());
  subscribe_homeassistant_state(
      &HomeAssistantSpeakerMediaPlayer::media_album_changed, this->entity_id_,
      "media_album_name");
  subscribe_homeassistant_state(
      &HomeAssistantSpeakerMediaPlayer::media_content_id_changed,
      this->entity_id_, "media_content_id");
  subscribe_homeassistant_state(
      &HomeAssistantSpeakerMediaPlayer::queue_position_changed,
      this->entity_id_, "queue_position");
  subscribe_homeassistant_state(
      &HomeAssistantSpeakerMediaPlayer::queue_size_changed, this->entity_id_,
      "queue_size");
}

void HomeAssistantSpeakerMediaPlayer::ungroup() {
  ESP_LOGI(TAG, "ungroup: %s", this->entity_id_.c_str());
  call_homeassistant_service("media_player.unjoin",
                             {
                                 {"entity_id", this->entity_id_},
                             });
}

void HomeAssistantSpeakerMediaPlayer::joinGroup(std::string newSpeakerName) {
  ESP_LOGI(TAG, "joinGroup: %s group to %s", this->entity_id_.c_str(),
           newSpeakerName.c_str());
  call_homeassistant_service("media_player.join",
                             {
                                 {"entity_id", newSpeakerName.c_str()},
                                 {"group_members", this->entity_id_.c_str()},
                             });
}

void HomeAssistantSpeakerMediaPlayer::updateVolumeLevel() {
  std::string entityIds = this->entity_id_;
  for (auto& speaker : groupMembers) {
    if (speaker != this->entity_id_) {
      entityIds = entityIds + ", " + speaker;
    }
  }
  ignore_api_updates_with_seconds(2);
  if (is_muted()) {
    // unmute all speakers
    call_homeassistant_service("media_player.volume_mute",
                               {
                                   {"entity_id", entityIds},
                                   {"is_volume_muted", "false"},
                               });
  }
  ESP_LOGI(TAG, "%s volume update %f group members %d", entityIds.c_str(),
           volume, groupMembers.size());
  call_homeassistant_service("media_player.volume_set",
                             {
                                 {"entity_id", entityIds},
                                 {"volume_level", to_string(volume)},
                             });
}

void HomeAssistantSpeakerMediaPlayer::clearSource() {
  HomeAssistantBaseMediaPlayer::clearSource();
  mediaPlaylist = "";
  mediaAlbum = "";
}

std::string HomeAssistantSpeakerMediaPlayer::mediaAlbumString() {
  if (mediaAlbum.length() > 0) {
    return mediaAlbum;
  }
  return "";
}

void HomeAssistantSpeakerMediaPlayer::media_album_changed(std::string state) {
  ESP_LOGI(TAG, "media_album_changed: %s changed to %s",
           this->entity_id_.c_str(), state.c_str());
  mediaAlbum = state.c_str();
  this->publish_state();
}

void HomeAssistantSpeakerMediaPlayer::media_content_id_changed(
    std::string state) {
  ESP_LOGI(TAG, "media_content_id_changed: %s changed to %s",
           this->entity_id_.c_str(), state.c_str());
  if (!can_update_from_api()) {
    return;
  }
  if (state.find("spdif") != std::string::npos) {
    mediaSource = TVRemotePlayerMediaSource;
    // clear mediaAlbum and mediaPlaylist when switching to TV
    mediaAlbum = "";
    mediaPlaylist = "";
  } else if (state.find("spotify") != std::string::npos) {
    mediaSource = SpotifyRemotePlayerMediaSource;
  } else {
    mediaSource = NoRemotePlayerMediaSource;
  }
  this->publish_state();
}

media_player::MediaPlayerTraits HomeAssistantSpeakerMediaPlayer::get_traits() {
  auto traits = media_player::MediaPlayerTraits();
  traits.set_supports_pause(true);
  return traits;
}

void HomeAssistantSpeakerMediaPlayer::queue_size_changed(std::string state) {
  ESP_LOGI(TAG, "queue_size_changed: %s changed to %s",
           this->entity_id_.c_str(), state.c_str());
  queueSize = atof(state.c_str());
  this->publish_state();
}

void HomeAssistantSpeakerMediaPlayer::queue_position_changed(
    std::string state) {
  ESP_LOGI(TAG, "queue_position_changed: %s changed to %s",
           this->entity_id_.c_str(), state.c_str());
  queuePosition = atof(state.c_str());
  this->publish_state();
}

}  // namespace homeassistant_media_player
}  // namespace esphome
