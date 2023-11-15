#pragma once
#include <string>

namespace esphome {
namespace homeassistant_media_player {

enum RemotePlayerMediaSource {
  NoRemotePlayerMediaSource,
  HomeRemotePlayerMediaSource,
  YouTubeRemotePlayerMediaSource,
  SpotifyRemotePlayerMediaSource,
  NetflixRemotePlayerMediaSource,
  PlexRemotePlayerMediaSource,
  TVRemotePlayerMediaSource
};

std::string playerSourceStateString(RemotePlayerMediaSource playingState);

}  // namespace homeassistant_media_player
}  // namespace esphome
