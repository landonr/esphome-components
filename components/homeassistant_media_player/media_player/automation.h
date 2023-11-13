#pragma once

#include "HomeAssistantTVMediaPlayer.h"
#include "esphome/core/automation.h"

namespace esphome {

namespace homeassistant_media_player {

#define MEDIA_PLAYE_TV_SIMPLE_COMMAND_ACTION(ACTION_CLASS, ACTION_COMMAND) \
  template <typename... Ts>                                                \
  class ACTION_CLASS : public Action<Ts...>,                               \
                       public Parented<HomeAssistantTVMediaPlayer> {       \
    void play(Ts... x) override {                                          \
      this->parent_->set_command(                                          \
          MediaPlayerTVRemoteCommand::                                     \
              MEDIA_PLAYER_TV_COMMAND_##ACTION_COMMAND);                   \
    }                                                                      \
  };

MEDIA_PLAYE_TV_SIMPLE_COMMAND_ACTION(UpAction, UP)
MEDIA_PLAYE_TV_SIMPLE_COMMAND_ACTION(DownAction, DOWN)
MEDIA_PLAYE_TV_SIMPLE_COMMAND_ACTION(LeftAction, LEFT)
MEDIA_PLAYE_TV_SIMPLE_COMMAND_ACTION(RightAction, RIGHT)
MEDIA_PLAYE_TV_SIMPLE_COMMAND_ACTION(SelectAction, SELECT)
MEDIA_PLAYE_TV_SIMPLE_COMMAND_ACTION(BackAction, BACK)
MEDIA_PLAYE_TV_SIMPLE_COMMAND_ACTION(HomeAction, HOME)
MEDIA_PLAYE_TV_SIMPLE_COMMAND_ACTION(PowerAction, POWER)

}  // namespace homeassistant_media_player
}  // namespace esphome
