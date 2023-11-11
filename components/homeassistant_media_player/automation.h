#pragma once

#include "esphome/core/automation.h"
#include "HomeAssistantMediaPlayerGroup.h"

namespace esphome {
namespace homeassistant_media_player {

template <typename... Ts>
class SelectNextMediaPlayerAction : public Action<Ts...> {
 public:
  explicit SelectNextMediaPlayerAction(HomeAssistantMediaPlayerGroup* mediaPlayerGroup) : mediaPlayerGroup_(mediaPlayerGroup) {}

  void play(Ts... x) override { this->mediaPlayerGroup_->selectNextMediaPlayer(); }

 protected:
  HomeAssistantMediaPlayerGroup* mediaPlayerGroup_;
};

}
}