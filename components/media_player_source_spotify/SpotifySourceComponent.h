#pragma once
#include <memory>
#include <string>
#include <vector>
#include "esphome/components/api/custom_api_device.h"
#include "esphome/components/media_player_source/MediaPlayerSourceBase.h"

namespace esphome {
namespace media_player_source_spotify {

class SpotifySourceComponent
    : public media_player_source::MediaPlayerSourceAPI {
 public:
  void setup() override;
  float get_setup_priority() const override { return setup_priority::LATE; }

 private:
  void playlists_changed(std::string state);
  void stripUnicode(std::string* str);
};
}  // namespace media_player_source_spotify
}  // namespace esphome
