#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "MediaPlayerSource.h"
#include "esphome/components/homeassistant_component/HomeAssistantComponent.h"
#include "esphome/components/media_player/media_player.h"
#include "esphome/components/media_player_source/MediaPlayerSourceBase.h"
#include "esphome/core/log.h"
#include "MediaPlayerSupportedFeature.h"

namespace esphome {
namespace homeassistant_media_player {

enum MediaPlayerRepeatMode { NOT_SET = 1, OFF = 2, ALL = 3, ONE = 4 };

enum RemotePlayerType { TVRemotePlayerType, SpeakerRemotePlayerType };

enum RemotePlayerState {
  NoRemotePlayerState,
  UnavailableRemotePlayerState,
  PowerOffRemotePlayerState,
  StoppedRemotePlayerState,
  PausedRemotePlayerState,
  PlayingRemotePlayerState
};

class HomeAssistantBaseMediaPlayer
    : public media_player::MediaPlayer,
      public homeassistant_component::HomeAssistantComponent {
 public:
  std::string mediaTitle = "";
  std::string mediaArtist = "";
  RemotePlayerMediaSource mediaSource = NoRemotePlayerMediaSource;
  RemotePlayerState playerState = NoRemotePlayerState;

  virtual HomeAssistantBaseMediaPlayer* get_parent_media_player() {
    return parent_media_player_;
  }

  void set_parent_media_player(
      HomeAssistantBaseMediaPlayer* parent_media_player) {
    parent_media_player_ = parent_media_player;
  }

  void register_custom_command(MediaPlayerCommand* command) {
    custom_commands_.push_back(command);
  }

  void register_source(media_player_source::MediaPlayerSourceBase* new_source);
  std::vector<media_player_source::MediaPlayerSourceBase*> sources;
  int index;
  virtual RemotePlayerType get_player_type() { return player_type_; }
  void setup() override;
  void playSource(media_player_source::MediaPlayerSourceItem* source);
  void playPause();
  void nextTrack();
  std::string mediaTitleString();
  std::string mediaSubtitleString();
  void clearMedia();
  bool is_muted() const override { return this->muted_; }
  bool is_shuffling() const { return this->shuffle_; }
  MediaPlayerRepeatMode repeat_mode() const { return this->repeat_mode_; }
  void toggle_shuffle();
  void toggle_mute();
  void toggle_repeat();
  std::vector<std::shared_ptr<MediaPlayerSupportedFeature>> get_features() {
    return supported_features_;
  }
  const std::vector<std::shared_ptr<MediaPlayerSupportedFeature>>* 
  get_option_menu_features(bool bottomMenu) {
    if (actionable_features_.size() > 0) {
      return &actionable_features_;
    }
    std::copy_if(supported_features_.begin(), supported_features_.end(),
                 std::back_inserter(actionable_features_),
                 [bottomMenu](std::shared_ptr<MediaPlayerSupportedFeature> i) {
                   switch (*(i.get())) {
                     case SHUFFLE_SET:
                     case GROUPING:
                     case REPEAT_SET:
                     case TURN_ON:
                     case TURN_OFF:
                     case TV_BACK:
                     case TV_HOME:
                     case MENU_HOME:
                       return true;
                     case PAUSE:
                     case VOLUME_SET:
                       return bottomMenu;
                     default:
                       return false;
                   }
                 });
    actionable_features_.push_back(std::make_shared<MediaPlayerSupportedFeature>(CUSTOM_COMMAND));
    return &actionable_features_;
  }

  bool supports(MediaPlayerSupportedFeature feature);

  virtual void increaseVolume();
  virtual void decreaseVolume();
  virtual void updateVolumeLevel();
  virtual void clearSource() {}
  virtual void media_artist_changed(std::string state);

  int mediaDuration = -1;
  int mediaPosition = -1;
  std::string playlist_title = "";
  std::vector<std::string> groupMembers;

  virtual void subscribe_source() {
    ESP_LOGI("homeassistant.media_player_base", "subscribe_source: %s",
             this->entity_id_.c_str());
  }
  virtual void subscribe_sources() {
    ESP_LOGI("homeassistant.media_player_base", "subscribe_sources: %s",
             this->entity_id_.c_str());
  }
  virtual void sources_changed(std::string state) {
    ESP_LOGI("homeassistant.media_player_base", "sources_changed: %s",
             this->entity_id_.c_str());
  }
  void media_title_changed(std::string state);

 protected:
  bool muted_ = false;
  bool shuffle_ = false;
  MediaPlayerRepeatMode repeat_mode_ = NOT_SET;
  float volume_step_ = 0.04;
  std::vector<std::shared_ptr<MediaPlayerSupportedFeature>>
      supported_features_ = {};
  std::vector<std::shared_ptr<MediaPlayerSupportedFeature>>
      actionable_features_ = {};
  std::vector<MediaPlayerCommand*>
      custom_commands_ = {};

  virtual void group_members_changed(std::string state);
  virtual void subscribe_media_artist();
  virtual void playMedia(media_player_source::MediaPlayerSourceItem* source);

 private:
  RemotePlayerType player_type_;
  HomeAssistantBaseMediaPlayer* parent_media_player_;

  void selectSource(media_player_source::MediaPlayerSourceItem* source);
  void tokenize(std::string const& str, std::string delim,
                std::vector<std::string>* out);
  std::string filter(std::string str);

  // subscriptions
  void subscribe_player_state();
  void subscribe_supported_features();
  void subscribe_media_title();
  void subscribe_muted();
  void subscribe_shuffle();
  void subscribe_repeat();
  void subscribe_volume();
  void subscribe_media_position();
  void subscribe_playlist();
  void subscribe_group_members();

  // changes
  void playerState_changed(std::string state);
  void player_supported_features_changed(std::string state);
  void muted_changed(std::string state);
  void shuffle_changed(std::string state);
  void repeat_changed(std::string state);
  void volume_changed(std::string state);
  void media_duration_changed(std::string state);
  void media_position_changed(std::string state);
  void playlist_changed(std::string state);
};
}  // namespace homeassistant_media_player
}  // namespace esphome
