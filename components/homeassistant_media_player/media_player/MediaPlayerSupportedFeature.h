#pragma once

#include <map>
#include <string>
#include <vector>
#include "MediaPlayerCommand.h"

namespace esphome {
namespace homeassistant_media_player {

enum MediaPlayerSupportedFeature {
  PAUSE = 1,
  SEEK = 2,
  VOLUME_SET = 4,
  VOLUME_MUTE = 8,
  PREVIOUS_TRACK = 16,
  NEXT_TRACK = 32,
  TURN_ON = 128,
  TURN_OFF = 256,
  PLAY_MEDIA = 512,
  VOLUME_STEP = 1024,
  SELECT_SOURCE = 2048,
  STOP = 4096,
  CLEAR_PLAYLIST = 8192,
  PLAY = 16384,
  SHUFFLE_SET = 32768,
  SELECT_SOUND_MODE = 65536,
  BROWSE_MEDIA = 131072,
  REPEAT_SET = 262144,
  GROUPING = 524288,

  // bonus features
  TV_BACK = 9,
  TV_HOME = 10,
  MENU_HOME = 11,
  REMOTE_MODE = 12,
  VOLUME_DOWN = 13,
  VOLUME_UP = 14,
  CUSTOM_COMMAND = 15,
  POWER_SET = 18,
};

std::string supported_feature_string(MediaPlayerSupportedFeature feature);

static std::vector<MediaPlayerSupportedFeature> supported_feature_bitmask_map =
    {PAUSE,          SEEK,        VOLUME_SET,
     VOLUME_MUTE,    VOLUME_DOWN, VOLUME_UP,
     PREVIOUS_TRACK, NEXT_TRACK,  TURN_ON,
     TURN_OFF,       PLAY_MEDIA,  VOLUME_STEP,
     SELECT_SOURCE,  STOP,        CLEAR_PLAYLIST,
     PLAY,           SHUFFLE_SET, SELECT_SOUND_MODE,
     BROWSE_MEDIA,   REPEAT_SET,  GROUPING,
     MENU_HOME};

static std::map<MediaPlayerSupportedFeature, std::string>
    supported_feature_string_map = {{PAUSE, "PAUSE"},
                                    {SEEK, "SEEK"},
                                    {VOLUME_SET, "VOLUME_SET"},
                                    {VOLUME_MUTE, "VOLUME_MUTE"},
                                    {VOLUME_DOWN, "VOLUME_DOWN"},
                                    {VOLUME_UP, "VOLUME_UP"},
                                    {PREVIOUS_TRACK, "PREVIOUS_TRACK"},
                                    {NEXT_TRACK, "NEXT_TRACK"},
                                    {TURN_ON, "TURN_ON"},
                                    {TURN_OFF, "TURN_OFF"},
                                    {PLAY_MEDIA, "PLAY_MEDIA"},
                                    {VOLUME_STEP, "VOLUME_STEP"},
                                    {SELECT_SOURCE, "SELECT_SOURCE"},
                                    {STOP, "STOP"},
                                    {CLEAR_PLAYLIST, "CLEAR_PLAYLIST"},
                                    {PLAY, "PLAY"},
                                    {SHUFFLE_SET, "SHUFFLE_SET"},
                                    {SELECT_SOUND_MODE, "SELECT_SOUND_MODE"},
                                    {BROWSE_MEDIA, "BROWSE_MEDIA"},
                                    {REPEAT_SET, "REPEAT_SET"},
                                    {GROUPING, "GROUPING"},
                                    {MENU_HOME, "MENU_HOME"},
                                    {CUSTOM_COMMAND, "CUSTOM_COMMAND"},
                                    {POWER_SET, "POWER_SET"}};

static std::map<std::string, MediaPlayerSupportedFeature>
    supported_feature_item_map = {{"PAUSE", PAUSE},
                                  {"SEEK", SEEK},
                                  {"VOLUME_SET", VOLUME_SET},
                                  {"VOLUME_MUTE", VOLUME_MUTE},
                                  {"VOLUME_UP", VOLUME_UP},
                                  {"VOLUME_DOWN", VOLUME_DOWN},
                                  {"PREVIOUS_TRACK", PREVIOUS_TRACK},
                                  {"NEXT_TRACK", NEXT_TRACK},
                                  {"TURN_ON", TURN_ON},
                                  {"TURN_OFF", TURN_OFF},
                                  {"PLAY_MEDIA", PLAY_MEDIA},
                                  {"VOLUME_STEP", VOLUME_STEP},
                                  {"SELECT_SOURCE", SELECT_SOURCE},
                                  {"STOP", STOP},
                                  {"CLEAR_PLAYLIST", CLEAR_PLAYLIST},
                                  {"PLAY", PLAY},
                                  {"SHUFFLE_SET", SHUFFLE_SET},
                                  {"SELECT_SOUND_MODE", SELECT_SOUND_MODE},
                                  {"BROWSE_MEDIA", BROWSE_MEDIA},
                                  {"REPEAT_SET", REPEAT_SET},
                                  {"GROUPING", GROUPING},
                                  {"MENU_HOME", MENU_HOME},
                                  {"CUSTOM_COMMAND", CUSTOM_COMMAND},
                                  {"POWER_SET", POWER_SET}};

class MediaPlayerFeatureCommand {
 public:
  MediaPlayerFeatureCommand(MediaPlayerSupportedFeature feature)
      : feature_(feature) {}
  void set_command(MediaPlayerCommand* command) { command_ = command; }
  MediaPlayerSupportedFeature get_feature() { return feature_; }
  MediaPlayerCommand* get_command() { return command_; }
  std::string get_title() {
    if (command_ != nullptr) {
      return command_->get_name();
    }
    return title_;
  }
  void set_title(std::string title) { title_ = title; }

 private:
  MediaPlayerSupportedFeature feature_;
  MediaPlayerCommand* command_ = nullptr;
  std::string title_;
};

}  // namespace homeassistant_media_player
}  // namespace esphome
