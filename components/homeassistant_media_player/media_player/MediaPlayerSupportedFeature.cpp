#include "MediaPlayerSupportedFeature.h"

namespace esphome {
namespace homeassistant_media_player {
std::string supported_feature_string(MediaPlayerSupportedFeature feature) {
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
    case POWER_SET:
      return "POWER_SET";
  }
  return "";
}
}  // namespace homeassistant_media_player
}  // namespace esphome
