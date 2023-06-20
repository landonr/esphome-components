#include "CustomSourceComponent.h"
#include <memory>
#include <vector>
#include "esphome/core/log.h"

namespace esphome {
namespace media_player_source_custom {

static const char* const TAG = "media.player.source.custom";

void CustomSourceComponent::setup() {}

void CustomSourceComponent::add_source(
    std::string name, std::string media_id,
    media_player_source::MediaPlayerSourceType media_type,
    media_player_source::AppPlayerSourceType app_type) {
  ESP_LOGI(TAG, "add_source %s", name.c_str());
  auto new_source = new media_player_source::MediaPlayerSourceItem(
      name, media_id, media_type);
  new_source->set_app_type(app_type);
  sources_.push_back(new_source);
}
}  // namespace media_player_source_custom
}  // namespace esphome
