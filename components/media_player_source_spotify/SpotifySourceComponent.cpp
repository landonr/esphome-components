#include "SpotifySourceComponent.h"
#include "esphome/core/log.h"

namespace esphome {
namespace media_player_source_spotify {

static const char* const TAG = "media.player.source.spotify";

void SpotifySourceComponent::setup() {
  ESP_LOGI(TAG, "setup spotify playlists");
  subscribe_homeassistant_state(&SpotifySourceComponent::playlists_changed,
                                "sensor.playlists_sensor", "playlists");
}

void SpotifySourceComponent::stripUnicode(std::string* str) {
  str->erase(remove_if(str->begin(), str->end(),
                       [](char c) { return !(c >= 0 && c < 128); }),
             str->end());
}

void SpotifySourceComponent::playlists_changed(std::string state) {
  stripUnicode(&state);
  ESP_LOGI(TAG, "Spotify playlists changes to %s", state.c_str());
  this->sources_.clear();
  parse_json_array(state, [this](JsonArray object) {
    for (JsonVariant v : object) {
      std::string key = v["uri"].as<std::string>();
      std::string value = v["name"].as<std::string>();
      ESP_LOGD(TAG, "new JSON key value %s %s", key.c_str(), value.c_str());
      auto newsource = new media_player_source::MediaPlayerSourceItem(
          value, key, media_player_source::MediaPlayerSourceTypePlaylist);
      this->sources_.push_back(newsource);
    }
  });
}
}  // namespace media_player_source_spotify
}  // namespace esphome
