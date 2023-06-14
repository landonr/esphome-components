#pragma once
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
};

static std::string supported_feature_string(
    MediaPlayerSupportedFeature feature) {
  switch (feature) {
    case PAUSE:
      return "Pause";
    case SEEK:
      return "Seek";
    case VOLUME_SET:
      return "Volume Set";
    case VOLUME_MUTE:
      return "Mute";
    case PREVIOUS_TRACK:
      return "Previous";
    case NEXT_TRACK:
      return "Next";
    case TURN_ON:
      return "Turn On";
    case TURN_OFF:
      return "Turn Off";
    case PLAY_MEDIA:
      return "Play Media";
    case VOLUME_STEP:
      return "Volume Step";
    case SELECT_SOURCE:
      return "Select Source";
    case STOP:
      return "Stop";
    case CLEAR_PLAYLIST:
      return "Clear Playlist";
    case PLAY:
      return "Play";
    case SHUFFLE_SET:
      return "Shuffle";
    case SELECT_SOUND_MODE:
      return "Select Sound Mode";
    case BROWSE_MEDIA:
      return "Browse Media";
    case REPEAT_SET:
      return "Repeat";
    case GROUPING:
      return "Grouping";
    case TV_BACK:
      return "Back";
    case TV_HOME:
      return "TV Home";
    case MENU_HOME:
      return "Menu";
    case REMOTE_MODE:
      return "Remote";
    case VOLUME_DOWN:
      return "Vol Dn";
    case VOLUME_UP:
      return "Vol Up";
    case CUSTOM_COMMAND:
      return "CSTM CMD";
  }
  return "";
}

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
                                    {CUSTOM_COMMAND, "CUSTOM_COMMAND"}};

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
                                  {"CUSTOM_COMMAND", CUSTOM_COMMAND}};

class MediaPlayerFeatureCommand {
 public:
  MediaPlayerFeatureCommand(MediaPlayerSupportedFeature feature,
                            const MediaPlayerCommand* command)
      : feature_(feature), command_(command) {}

private:
  MediaPlayerSupportedFeature feature_;
  const MediaPlayerCommand* command_;
};
}  // namespace homeassistant_media_player
}  // namespace esphome
