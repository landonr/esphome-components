#pragma once
#include <memory>
#include <string>
#include <vector>
#include "esphome/components/api/custom_api_device.h"
#include "esphome/core/component.h"

namespace esphome {
namespace media_player_source {

enum MediaPlayerSourceType {
  MediaPlayerSourceTypeMusic,
  MediaPlayerSourceTypeFavoriteItemID,
  MediaPlayerSourceTypeSource,
  MediaPlayerSourceTypePlaylist,
  MediaPlayerSourceTypeApp,
  MediaPlayerSourceTypeChannel,
  MediaPlayerSourceTypeURL
};

enum AppPlayerSourceType {
  AppPlayerSourceTypeNone,
  AppPlayerSourceTypeYouTube,
  AppPlayerSourceTypeSpotify,
  AppPlayerSourceTypeNetflix,
  AppPlayerSourceTypePlex,
  AppPlayerSourceTypeDisneyPlus,
  AppPlayerSourceTypeHulu,
  AppPlayerSourceTypePrimeVideo,
  AppPlayerSourceTypeHBO
};

class MediaPlayerSourceItem {
 public:
  MediaPlayerSourceItem(std::string new_name, std::string new_media_content_id,
                        MediaPlayerSourceType new_media_type)
      : name_(new_name),
        media_content_id_(new_media_content_id),
        media_type_(new_media_type) {}
  std::string get_name() { return name_; }
  std::string get_media_content_id() { return media_content_id_; }
  MediaPlayerSourceType get_media_type() { return media_type_; }
  void set_name(const std::string& name) { name_ = name; }
  void set_media_content_id(const std::string& media_content_id) {
    media_content_id_ = media_content_id;
  }
  void set_media_type(const MediaPlayerSourceType& media_type) {
    media_type_ = media_type;
  }
  void set_app_type(const AppPlayerSourceType& app_type) {
    app_type_ = app_type;
  }
  AppPlayerSourceType get_app_type() { return app_type_; }

  std::string sourceTypeString() {
    switch (media_type_) {
      case MediaPlayerSourceTypeMusic:
        return "music";
      case MediaPlayerSourceTypeFavoriteItemID:
        return "favorite_item_id";
      case MediaPlayerSourceTypeSource:
        return "source";
      case MediaPlayerSourceTypePlaylist:
        return "playlist";
      case MediaPlayerSourceTypeApp:
        return "app";
      case MediaPlayerSourceTypeChannel:
        return "channel";
      case MediaPlayerSourceTypeURL:
        return "url";
    }
    return "";
  }

  std::string sourceAppString() {
    switch (app_type_) {
      case AppPlayerSourceTypeNone:
        return "none";
      case AppPlayerSourceTypeYouTube:
        return "youtube";
      case AppPlayerSourceTypeSpotify:
        return "spotify";
      case AppPlayerSourceTypeNetflix:
        return "netflix";
      case AppPlayerSourceTypePlex:
        return "plex";
      case AppPlayerSourceTypeDisneyPlus:
        return "disney_plus";
      case AppPlayerSourceTypeHulu:
        return "hulu";
      case AppPlayerSourceTypePrimeVideo:
        return "prime_video";
      case AppPlayerSourceTypeHBO:
        return "hbo";
    }
    return "";
  }

 protected:
  std::string name_ = "no";
  std::string media_content_id_ = "no";
  MediaPlayerSourceType media_type_;
  AppPlayerSourceType app_type_ = AppPlayerSourceTypeNone;
};

class MediaPlayerSourceBase : public EntityBase {
 public:
  // void set_entity_id(const std::string& entity_id) { entity_id_ = entity_id; }
  // std::string get_entity_id() { return entity_id_; }
  std::vector<MediaPlayerSourceItem*>* get_sources() { return &sources_; }

 protected:
  std::vector<MediaPlayerSourceItem*> sources_ = {};
  using json_parse_array_t = std::function<void(JsonArray)>;
  void parse_json_array(const std::string& data, const json_parse_array_t& f);
};

class MediaPlayerSourceAPI : public MediaPlayerSourceBase,
                             public Component,
                             public api::CustomAPIDevice {
 public:
  void set_entity_id(const std::string& entity_id) { entity_id_ = entity_id; }
  std::string get_entity_id() { return entity_id_; }
  float get_setup_priority() const override { return setup_priority::LATE; }

 protected:
  std::string entity_id_;
};

class MediaPlayerSourceInternal : public MediaPlayerSourceBase {
 public:
  void set_sources(const std::vector<MediaPlayerSourceItem*>& sources) {
    sources_ = sources;
  }

  void set_sources_json_array(const std::string state) {
    for (auto source : this->sources_) {
      delete source;
    }
    this->sources_.clear();
    parse_json_array(state, [this](JsonArray object) {
      for (JsonVariant v : object) {
        std::string sourceName = v.as<std::string>();
        auto newsource = new media_player_source::MediaPlayerSourceItem(
            sourceName, sourceName,
            media_player_source::MediaPlayerSourceType::
                MediaPlayerSourceTypeSource);
        this->sources_.push_back(newsource);
      }
    });
  }
};

}  // namespace media_player_source
}  // namespace esphome
