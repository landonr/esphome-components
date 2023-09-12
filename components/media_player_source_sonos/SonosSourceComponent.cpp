#include "SonosSourceComponent.h"
#include "esphome/core/log.h"

namespace esphome {
namespace media_player_source_sonos {

static const char* const TAG = "media.player.source.sonos";

void SonosSourceComponent::setup() {
  ESP_LOGI(TAG, "setup sonos favorites");
  subscribe_homeassistant_state(&SonosSourceComponent::sonos_favorites_changed,
                                "sensor.sonos_favorites", "items");
}

void SonosSourceComponent::sonos_favorites_changed(std::string state) {
  ESP_LOGI(TAG, "Sonos Favorites changes to %s", state.c_str());
  json::parse_json(state, [this](JsonObject array) {
    this->sources_.clear();
    int index = 0;
    for (JsonPair v : array) {
      if (index >= limit_) {
        return;
      }
      std::string value = v.value().as<std::string>();
      std::string key = v.key().c_str();
      ESP_LOGD("group", "new JSON key value %s %s", key.c_str(), value.c_str());
      auto newsource = new media_player_source::MediaPlayerSourceItem(
          value, key, media_player_source::MediaPlayerSourceTypeFavoriteItemID);
      this->sources_.push_back(newsource);
      index++;
    }
    array.clear();
  });
}
}  // namespace media_player_source_sonos
}  // namespace esphome
