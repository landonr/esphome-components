#pragma once
#include <memory>
#include <string>
#include <vector>
#include "esphome/components/api/custom_api_device.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/media_player/media_player.h"
#include "esphome/components/media_player_source/MediaPlayerSourceBase.h"
#include "esphome/components/sensor/sensor.h"
#include "media_player/HomeAssistantBaseMediaPlayer.h"
#include "media_player/HomeAssistantSpeakerMediaPlayer.h"
#include "media_player/HomeAssistantTVMediaPlayer.h"

namespace esphome {
namespace homeassistant_media_player {

class SpeakerSetup {
 public:
  SpeakerSetup(std::string newEntityId, std::string newFriendlyName)
      : entity_id_(newEntityId), friendlyName(newFriendlyName) {}
  std::string entity_id_;
  std::string friendlyName;
};

class TVSetup {
 public:
  TVSetup(std::string newEntityId, std::string newFriendlyName,
          SpeakerSetup* newSoundBar)
      : entity_id_(newEntityId),
        friendlyName(newFriendlyName),
        soundBar(newSoundBar) {}
  std::string entity_id_;
  std::string friendlyName;
  SpeakerSetup* soundBar;
};

class HomeAssistantMediaPlayerGroup : public HomeAssistantTVMediaPlayer {
 public:
  HomeAssistantBaseMediaPlayer* active_player_ = NULL;
  int loadedPlayers = 0;
  HomeAssistantBaseMediaPlayer* newSpeakerGroupParent = NULL;

  int totalPlayers() { return media_players_.size(); }
  void selectFirstActivePlayer();
  void setup();
  HomeAssistantBaseMediaPlayer* get_active_player() { return active_player_; }
  void findActivePlayer(bool background = false);
  bool selectMediaPlayers(HomeAssistantBaseMediaPlayer* selected_media_player);
  void setActivePlayer(HomeAssistantBaseMediaPlayer* newActivePlayer);
  void register_media_player(HomeAssistantBaseMediaPlayer* new_media_player);
  void increaseSpeakerVolume();
  void decreaseSpeakerVolume();
  bool mediaShuffling();
  MediaPlayerRepeatMode get_repeat_mode() {
    if (active_player_ != NULL) {
      return active_player_->repeat_mode();
    }
    return MediaPlayerRepeatMode::NOT_SET;
  }
  void toggle_shuffle();
  void toggle_mute();
  void toggle_repeat();
  std::string shuffle_string();
  std::string muteString();
  double getVolumeLevel();
  bool selectGroup(HomeAssistantBaseMediaPlayer* selected_group_parent,
                   int menuIndex);
  bool updateMediaPosition();
  void selectNextMediaPlayer();
  std::string playTitleString();
  std::string mediaTitleString();
  std::string mediaSubtitleString();
  std::string mediaAlbumString();
  std::string mediaPlaylistString();
  std::string queuePositionString();
  void sendActivePlayerRemoteCommand(MediaPlayerTVRemoteCommand command);
  void call_feature(MediaPlayerSupportedFeature feature);
  std::vector<media_player_source::MediaPlayerSourceBase*>*
  activePlayerSources();
  void playSource(media_player_source::MediaPlayerSourceItem* source);
  float get_setup_priority() const override { return setup_priority::LATE; }
  void set_active_player_source_index(int active_player_source_index) {
    active_player_source_index_ = active_player_source_index;
  }
  int get_active_player_source_index() { return active_player_source_index_; }
  const std::vector<HomeAssistantBaseMediaPlayer*>* get_media_players() {
    return &media_players_;
  }
  std::string get_new_source_name() {
    std::string result = new_source_name;
    return result;
  }
  void set_finished_loading_sensor(
      binary_sensor::BinarySensor* finished_loading_sensor) {
    finished_loading_sensor_ = finished_loading_sensor;
  }
  void set_active_player_text_sensor(
      text_sensor::TextSensor* active_player_text_sensor) {
    active_player_text_sensor_ = active_player_text_sensor;
  }
  void control(const media_player::MediaPlayerCall& call) override;
  std::string stringForRemoteCommand(
      MediaPlayerTVRemoteCommand command) override {
    return "";
  }
  void tvRemoteCommand(MediaPlayerTVRemoteCommand command) override;
  void togglePower();

 private:
  std::vector<HomeAssistantBaseMediaPlayer*> media_players_;
  void state_updated(HomeAssistantBaseMediaPlayer* player);
  int active_player_source_index_ = -1;
  std::string new_source_name = "";
  binary_sensor::BinarySensor* finished_loading_sensor_ = NULL;
  text_sensor::TextSensor* active_player_text_sensor_ = NULL;
};

}  // namespace homeassistant_media_player
}  // namespace esphome
