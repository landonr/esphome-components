#pragma once
#include <memory>
#include <string>
#include <vector>
#include "esphome/components/api/custom_api_device.h"
#include "esphome/components/media_player_source/MediaPlayerSourceBase.h"

namespace esphome {
namespace media_player_source_sonos {

class SonosSourceComponent : public media_player_source::MediaPlayerSourceAPI {
 public:
  void setup() override;
  float get_setup_priority() const override { return setup_priority::LATE; }
  void set_limit(int limit) { limit_ = limit; }

 private:
  void sonos_favorites_changed(std::string state);
  int limit_ = 100;
};

}  // namespace media_player_source_sonos
}  // namespace esphome
