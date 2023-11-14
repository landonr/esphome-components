#pragma once

#include "HomeAssistantMediaPlayerGroup.h"
#include "esphome/core/automation.h"

namespace esphome {
namespace homeassistant_media_player {

template <typename... Ts>
class SelectNextMediaPlayerAction : public Action<Ts...> {
 public:
  explicit SelectNextMediaPlayerAction(
      HomeAssistantMediaPlayerGroup* mediaPlayerGroup)
      : mediaPlayerGroup_(mediaPlayerGroup) {}

  void play(Ts... x) override {
    this->mediaPlayerGroup_->selectNextMediaPlayer();
  }

 protected:
  HomeAssistantMediaPlayerGroup* mediaPlayerGroup_;
};

}  // namespace homeassistant_media_player
}  // namespace esphome