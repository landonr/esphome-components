#include "HomeAssistantMediaPlayerGroup.h"
#include <algorithm>

namespace esphome {
namespace homeassistant_media_player {

static const char* const TAG = "homeassistant.media_player_group";

void HomeAssistantMediaPlayerGroup::setup() {
  ESP_LOGI(TAG, "Subscribe states");
}

void HomeAssistantMediaPlayerGroup::register_media_player(
    HomeAssistantBaseMediaPlayer* new_media_player) {
  new_media_player->index = media_players_.size();
  media_players_.push_back(new_media_player);

  new_media_player->add_on_state_callback(
      [this, new_media_player]() { this->state_updated(new_media_player); });
}

bool HomeAssistantMediaPlayerGroup::selectMediaPlayers(
    HomeAssistantBaseMediaPlayer* selected_media_player) {
  ESP_LOGI(TAG, "selecting media player %s",
           selected_media_player->get_entity_id().c_str());
  for (auto& media_player : media_players_) {
    ESP_LOGI(TAG, "selecting mediar player? %s",
             media_player->get_entity_id().c_str());

    if (media_player->get_entity_id() ==
        selected_media_player->get_entity_id()) {
      setActivePlayer(media_player);
      return true;
    }
  }
  return false;
}

void HomeAssistantMediaPlayerGroup::selectFirstActivePlayer() {
  if (playerSearchFinished || loadedPlayers < 1) {
    return;
  }
  for (auto& speaker : media_players_) {
    if (speaker->playerState != NoRemotePlayerState) {
      playerSearchFinished = true;
      setActivePlayer(speaker);
      // display->updateDisplay(true);
      return;
    }
  }
}

void HomeAssistantMediaPlayerGroup::findActivePlayer(bool background) {
  if (playerSearchFinished) {
    return;
  }
  HomeAssistantBaseMediaPlayer* newActivePlayer = NULL;
  int tempLoadedPlayers = 0;
  for (auto& media_player : media_players_) {
    if (media_player->get_player_type() ==
        homeassistant_media_player::RemotePlayerType::SpeakerRemotePlayerType) {
      HomeAssistantSpeakerMediaPlayer* speaker =
          static_cast<HomeAssistantSpeakerMediaPlayer*>(media_player);
      if (speaker && speaker->playerState == NoRemotePlayerState) {
        ESP_LOGD(TAG, "findActivePlayer: didnt load player %s",
                 media_player->get_entity_id().c_str());
        // try again later
        return;
      } else if (speaker->mediaSource == TVRemotePlayerMediaSource &&
                 speaker->get_tv()) {
        if (!newActivePlayer ||
            (newActivePlayer &&
             newActivePlayer->playerState < speaker->get_tv()->playerState)) {
          // set player to speaker's soundbar
          newActivePlayer = speaker->get_tv();
        }
      } else if (newActivePlayer) {
        if (newActivePlayer->playerState < speaker->playerState &&
            speaker->mediaSource != TVRemotePlayerMediaSource &&
            !(newActivePlayer->get_player_type() ==
                  homeassistant_media_player::RemotePlayerType::
                      TVRemotePlayerType &&
              newActivePlayer->playerState > PowerOffRemotePlayerState)) {
          // set player only if active player isn't a TV that's turned on
          ESP_LOGD(TAG, "findActivePlayer: set player %s %d %d",
                   media_player->get_entity_id().c_str(),
                   newActivePlayer->playerState, speaker->playerState);
          newActivePlayer = speaker;
        }
      } else {
        ESP_LOGD(TAG, "findActivePlayer: set player %s",
                 media_player->get_entity_id().c_str());
        // set default active player
        newActivePlayer = speaker;
      }
      tempLoadedPlayers++;
      ESP_LOGI(TAG, "findActivePlayer: loaded player %s",
               media_player->get_entity_id().c_str());
      loadedPlayers = std::max(tempLoadedPlayers, loadedPlayers);
    } else {
      HomeAssistantTVMediaPlayer* tv =
          static_cast<HomeAssistantTVMediaPlayer*>(media_player);
      if (tv->playerState == NoRemotePlayerState) {
        // try again later
        return;
      } else if (newActivePlayer) {
        if (newActivePlayer->playerState < tv->playerState) {
          ESP_LOGD(TAG, "findActivePlayer: set player %s",
                   media_player->get_entity_id().c_str());
          // set player to tv because it's state is greater
          newActivePlayer = tv;
        }
      } else {
        ESP_LOGI(TAG, "findActivePlayer: set player %s",
                 media_player->get_entity_id().c_str());
        // set player to tv because it's default
        newActivePlayer = tv;
      }
      tempLoadedPlayers++;
      ESP_LOGD(TAG, "findActivePlayer: loaded player %s",
               media_player->get_entity_id().c_str());
      loadedPlayers = std::max(tempLoadedPlayers, loadedPlayers);
    }
  }
  if (newActivePlayer) {
    ESP_LOGI(TAG, "setting active player %s",
             newActivePlayer->get_entity_id().c_str());
    setActivePlayer(newActivePlayer);
    playerSearchFinished = true;
    if (!background) {
      // display->updateDisplay(true);
    }
  }
}

void HomeAssistantMediaPlayerGroup::setActivePlayer(
    HomeAssistantBaseMediaPlayer* newActivePlayer) {
  ESP_LOGI(TAG, "New active player %s",
           newActivePlayer->get_entity_id().c_str());
  active_player_ = newActivePlayer;
}

void HomeAssistantMediaPlayerGroup::increaseSpeakerVolume() {
  if (active_player_->get_player_type() ==
      homeassistant_media_player::RemotePlayerType::SpeakerRemotePlayerType) {
    HomeAssistantSpeakerMediaPlayer* activeSpeaker =
        static_cast<HomeAssistantSpeakerMediaPlayer*>(active_player_);
    if (activeSpeaker != NULL) {
      activeSpeaker->increaseVolume();
    }
  } else {
    HomeAssistantTVMediaPlayer* activeTV =
        static_cast<HomeAssistantTVMediaPlayer*>(active_player_);
    if (activeTV != NULL) {
      if (activeTV->get_parent_media_player() != NULL) {
        HomeAssistantSpeakerMediaPlayer* tvSpeaker =
            static_cast<HomeAssistantSpeakerMediaPlayer*>(
                activeTV->get_parent_media_player());
        tvSpeaker->increaseVolume();
      } else {
        activeTV->increaseVolume();
      }
    }
  }
}

void HomeAssistantMediaPlayerGroup::decreaseSpeakerVolume() {
  if (active_player_->get_player_type() ==
      homeassistant_media_player::RemotePlayerType::SpeakerRemotePlayerType) {
    HomeAssistantSpeakerMediaPlayer* activeSpeaker =
        static_cast<HomeAssistantSpeakerMediaPlayer*>(active_player_);
    if (activeSpeaker != NULL) {
      activeSpeaker->decreaseVolume();
    }
  } else {
    HomeAssistantTVMediaPlayer* activeTV =
        static_cast<HomeAssistantTVMediaPlayer*>(active_player_);
    if (activeTV != NULL) {
      if (activeTV->get_parent_media_player() != NULL) {
        HomeAssistantSpeakerMediaPlayer* tvSpeaker =
            static_cast<HomeAssistantSpeakerMediaPlayer*>(
                activeTV->get_parent_media_player());
        tvSpeaker->decreaseVolume();
      } else {
        activeTV->decreaseVolume();
      }
    }
  }
}

bool HomeAssistantMediaPlayerGroup::mediaShuffling() {
  HomeAssistantSpeakerMediaPlayer* activeSpeaker =
      static_cast<HomeAssistantSpeakerMediaPlayer*>(active_player_);
  if (activeSpeaker != NULL) {
    if (activeSpeaker->is_shuffling()) {
      return true;
    } else {
      return false;
    }
  }
  return false;
}

void HomeAssistantMediaPlayerGroup::toggle_shuffle() {
  HomeAssistantSpeakerMediaPlayer* activeSpeaker =
      static_cast<HomeAssistantSpeakerMediaPlayer*>(active_player_);
  if (activeSpeaker != NULL) {
    activeSpeaker->toggle_shuffle();
  }
}

void HomeAssistantMediaPlayerGroup::toggle_repeat() {
  HomeAssistantSpeakerMediaPlayer* activeSpeaker =
      static_cast<HomeAssistantSpeakerMediaPlayer*>(active_player_);
  if (activeSpeaker != NULL) {
    activeSpeaker->toggle_repeat();
  }
}

void HomeAssistantMediaPlayerGroup::toggle_mute() {
  HomeAssistantSpeakerMediaPlayer* activeSpeaker =
      static_cast<HomeAssistantSpeakerMediaPlayer*>(active_player_);
  if (activeSpeaker != NULL) {
    activeSpeaker->toggle_mute();
  }
}

std::string HomeAssistantMediaPlayerGroup::shuffle_string() {
  if (mediaShuffling()) {
    return "Shfl On";
  } else {
    return "Shfl Off";
  }
}

std::string HomeAssistantMediaPlayerGroup::muteString() {
  HomeAssistantSpeakerMediaPlayer* activeSpeaker =
      static_cast<HomeAssistantSpeakerMediaPlayer*>(active_player_);
  if (activeSpeaker != NULL) {
    if (activeSpeaker->is_muted()) {
      return "Unmute";
    } else {
      return "Mute";
    }
  }
  return "";
}

double HomeAssistantMediaPlayerGroup::getVolumeLevel() {
  if (active_player_ == nullptr) {
    ESP_LOGI(TAG, "getVolumeLevel: no active player");
    return 0;
  }
  if (active_player_->get_player_type() ==
      homeassistant_media_player::RemotePlayerType::TVRemotePlayerType) {
    HomeAssistantTVMediaPlayer* activeTV =
        static_cast<HomeAssistantTVMediaPlayer*>(active_player_);
    if (activeTV != NULL) {
      if (activeTV->get_soundbar() != NULL) {
        HomeAssistantSpeakerMediaPlayer* tvSpeaker =
            static_cast<HomeAssistantSpeakerMediaPlayer*>(
                activeTV->get_soundbar());
        if (tvSpeaker->volume != -1) {
          double volume = tvSpeaker->volume * 100;
          return volume;
        } else {
          ESP_LOGI(TAG, "getVolumeLevel: tv volume isnt -1 %f",
                   tvSpeaker->volume);
        }
      } else {
        ESP_LOGI(TAG, "getVolumeLevel: tv has no parent");
        return -1;
      }
    } else {
      ESP_LOGI(TAG, "getVolumeLevel: player isnt tv");
    }
  } else {
    HomeAssistantSpeakerMediaPlayer* activeSpeaker =
        static_cast<HomeAssistantSpeakerMediaPlayer*>(active_player_);
    if (activeSpeaker != NULL) {
      if (activeSpeaker->volume != -1) {
        double volume = activeSpeaker->volume * 100;
        return volume;
      }
    }
  }
  return 0;
}

bool HomeAssistantMediaPlayerGroup::selectGroup(
    HomeAssistantBaseMediaPlayer* selected_group_parent, int menuIndex) {
  if (newSpeakerGroupParent == nullptr) {
    newSpeakerGroupParent = selected_group_parent;
    return false;
  } else if (menuIndex == 0) {
    newSpeakerGroupParent = nullptr;
    return false;
  }

  std::vector<HomeAssistantSpeakerMediaPlayer*> speakerList;
  for (auto& media_player : media_players_) {
    if (media_player->get_entity_id() ==
            newSpeakerGroupParent->get_entity_id() ||
        media_player->get_player_type() != SpeakerRemotePlayerType ||
        !media_player->supports(homeassistant_media_player::
                                    MediaPlayerSupportedFeature::GROUPING)) {
      continue;
    }
    HomeAssistantSpeakerMediaPlayer* speaker =
        static_cast<HomeAssistantSpeakerMediaPlayer*>(media_player);
    speakerList.push_back(speaker);
  }
  auto speaker = speakerList[menuIndex - 1];
  auto groupMembers = speaker->get_group_members();

  if (groupMembers->size() > 1) {
    if (std::find(groupMembers->begin(), groupMembers->end(),
                  newSpeakerGroupParent->get_entity_id()) !=
        groupMembers->end()) {
      speaker->ungroup();
    }
  } else {
    speaker->joinGroup(newSpeakerGroupParent->get_entity_id());
  }
  return true;
}

bool HomeAssistantMediaPlayerGroup::updateMediaPosition() {
  if (!playerSearchFinished) {
    return false;
  }
  bool updateDisplay = false;
  for (auto& media_player : media_players_) {
    if (media_player->get_player_type() != SpeakerRemotePlayerType) {
      continue;
    }
    HomeAssistantSpeakerMediaPlayer* speaker =
        static_cast<HomeAssistantSpeakerMediaPlayer*>(media_player);
    if (speaker->playerState == PlayingRemotePlayerState &&
        speaker->mediaDuration >= 0 && speaker->mediaPosition >= 0 &&
        speaker->mediaPosition < speaker->mediaDuration) {
      speaker->mediaPosition++;
      updateDisplay = true;
    }
  }
  if (active_player_ != NULL) {
    switch (active_player_->get_player_type()) {
      case homeassistant_media_player::RemotePlayerType::TVRemotePlayerType:
        updateDisplay = false;
        break;
      case homeassistant_media_player::RemotePlayerType::
          SpeakerRemotePlayerType:
        break;
    }
  }
  return updateDisplay;
}

void HomeAssistantMediaPlayerGroup::selectNextMediaPlayer() {
  if (active_player_ != NULL) {
    ESP_LOGI(TAG, "Syncing active player %d, %d", active_player_->index,
             media_players_.size());
    if (active_player_->index < media_players_.size() - 1) {
      ESP_LOGI(TAG, "32Syncing active player %d", active_player_->index);
      setActivePlayer(media_players_[active_player_->index + 1]);
    } else if (media_players_.size() > 0) {
      ESP_LOGI(TAG, "323 Syncing active player %d", active_player_->index);
      setActivePlayer(media_players_[0]);
    } else {
      ESP_LOGI(TAG, "132Syncing active player %d", active_player_->index);
    }
  }
}

std::string HomeAssistantMediaPlayerGroup::playTitleString() {
  switch (active_player_->playerState) {
    case homeassistant_media_player::RemotePlayerState::PausedRemotePlayerState:
      return "Play";
    case homeassistant_media_player::RemotePlayerState::
        PlayingRemotePlayerState:
      return "Pause";
    case homeassistant_media_player::RemotePlayerState::
        StoppedRemotePlayerState:
    case homeassistant_media_player::RemotePlayerState::NoRemotePlayerState:
      return "Stopped";
    case homeassistant_media_player::RemotePlayerState::
        PowerOffRemotePlayerState:
      return "Off";
    case homeassistant_media_player::RemotePlayerState::
        UnavailableRemotePlayerState:
      return "Unavailable";
  }
  return "";
}

std::string HomeAssistantMediaPlayerGroup::mediaTitleString() {
  switch (active_player_->get_player_type()) {
    case homeassistant_media_player::RemotePlayerType::TVRemotePlayerType:
      return active_player_->mediaTitleString();
    case homeassistant_media_player::RemotePlayerType::SpeakerRemotePlayerType:
      if (active_player_->mediaSource == TVRemotePlayerMediaSource) {
        HomeAssistantSpeakerMediaPlayer* activeSpeaker =
            static_cast<HomeAssistantSpeakerMediaPlayer*>(active_player_);
        if (activeSpeaker != NULL && activeSpeaker->get_tv() != NULL) {
          return activeSpeaker->get_tv()->mediaTitleString();
        }
      }
      return active_player_->mediaTitleString();
  }
  return "";
}

std::string HomeAssistantMediaPlayerGroup::mediaSubtitleString() {
  switch (active_player_->get_player_type()) {
    case homeassistant_media_player::RemotePlayerType::TVRemotePlayerType:
      return active_player_->mediaSubtitleString();
    case homeassistant_media_player::RemotePlayerType::SpeakerRemotePlayerType:
      if (active_player_->mediaSource == TVRemotePlayerMediaSource) {
        return playerSourceStateString(active_player_->mediaSource);
      }
      return active_player_->mediaSubtitleString();
  }
  return "";
}

void HomeAssistantMediaPlayerGroup::sendActivePlayerRemoteCommand(
    MediaPlayerTVRemoteCommand command) {
  HomeAssistantTVMediaPlayer* activeTV =
      static_cast<HomeAssistantTVMediaPlayer*>(active_player_);
  if (activeTV != NULL) {
    activeTV->tvRemoteCommand(command);
  }
}

void HomeAssistantMediaPlayerGroup::call_feature(
    MediaPlayerSupportedFeature feature) {
  switch (feature) {
    case TURN_ON:
    case TURN_OFF:
    case POWER_SET:
      sendActivePlayerRemoteCommand(POWER);
      break;
    case SHUFFLE_SET:
      toggle_shuffle();
      break;
    case VOLUME_MUTE:
      toggle_mute();
      break;
    case TV_BACK:
      sendActivePlayerRemoteCommand(BACK);
      break;
    case TV_HOME:
      sendActivePlayerRemoteCommand(HOME);
      break;
    case PAUSE:
      active_player_->playPause();
      break;
    case REPEAT_SET:
      toggle_repeat();
      break;
    case NEXT_TRACK:
      active_player_->nextTrack();
      break;
    case VOLUME_DOWN:
      decreaseSpeakerVolume();
      break;
    case VOLUME_UP:
      increaseSpeakerVolume();
      break;
    case MENU_HOME:
    case VOLUME_SET:
    case SEEK:
    case SELECT_SOURCE:
    case CLEAR_PLAYLIST:
    case GROUPING:
    case SELECT_SOUND_MODE:
    case BROWSE_MEDIA:
    case PLAY_MEDIA:
    case VOLUME_STEP:
    case STOP:
    case PLAY:
    case PREVIOUS_TRACK:
    default:
      break;
  }
}

std::vector<media_player_source::MediaPlayerSourceBase*>*
HomeAssistantMediaPlayerGroup::activePlayerSources() {
  return active_player_->get_sources();
}

// private

void HomeAssistantMediaPlayerGroup::syncActivePlayer(RemotePlayerState state) {
  if (loadedPlayers < totalPlayers()) {
    return;
  }
  ESP_LOGI(TAG, "Syncing active player %d", state);
  playerSearchFinished = false;
  findActivePlayer(true);
}

void HomeAssistantMediaPlayerGroup::state_updated(
    HomeAssistantBaseMediaPlayer* player) {
  ESP_LOGD(TAG, "state update callback %d %d", active_player_ == NULL,
           sync_active_player);
  bool playerGrouped = player->get_group_members()->size() > 1;
  bool parentSet = player->get_parent_media_player() != NULL;
  bool setParent = !parentSet && playerGrouped;
  bool unsetParent = parentSet && !playerGrouped;
  bool entityIsParent = parentSet && player->get_group_members()->size() > 0 &&
                        player->get_parent_media_player()->get_entity_id() ==
                            (*player->get_group_members())[0];
  bool playerParentNotSynced = (setParent || unsetParent) && !entityIsParent;
  ESP_LOGD(TAG,
           "trying group parent: set parent %d, unset parent %d, entity is "
           "parent %d",
           setParent, unsetParent, entityIsParent);
  if (playerParentNotSynced) {
    ESP_LOGD(TAG, "setting group parent: %s, %d",
             player->get_entity_id().c_str(),
             player->get_group_members()->size());
    for (auto& media_player : media_players_) {
      if (media_player->get_entity_id() == player->get_entity_id()) {
        continue;
      } else if (player->get_group_members()->size() > 0 &&
                 media_player->get_entity_id() ==
                     (*player->get_group_members())[0]) {
        player->set_parent_media_player(media_player);
        ESP_LOGD(TAG, "set group parent: %s, %s",
                 player->get_entity_id().c_str(),
                 media_player->get_entity_id().c_str());
        break;
      } else if (player->get_group_members()->size() <= 1) {
        player->set_parent_media_player(NULL);
        ESP_LOGD(TAG, "unset group parent: %s, %s",
                 player->get_entity_id().c_str(),
                 media_player->get_entity_id().c_str());
      }
    }
  }
  if (active_player_ != NULL) {
    if (active_player_ == player) {
      publish_state(0);
    }
    return;
  } else if (!sync_active_player) {
    publish_state(0);
    return;
  }

  auto state = player->playerState;
  ESP_LOGD(TAG,
           "Trying to sync active player, state: %d activePlayerNull: %d, "
           "sync_active_player: %d",
           state, active_player_ == NULL, sync_active_player == true);
  switch (state) {
    case homeassistant_media_player::RemotePlayerState::NoRemotePlayerState:
    case homeassistant_media_player::RemotePlayerState::PausedRemotePlayerState:
    case homeassistant_media_player::RemotePlayerState::
        UnavailableRemotePlayerState:
      ESP_LOGD(TAG, "Trying to sync active player - 1");
      syncActivePlayer(state);
      break;
    case homeassistant_media_player::RemotePlayerState::
        PlayingRemotePlayerState:
      if (active_player_ == NULL) {
        ESP_LOGD(TAG, "Trying to sync active player - 2");
        syncActivePlayer(state);
      } else if (active_player_->playerState < state) {
        ESP_LOGD(TAG, "Trying to sync active player - 3 %d",
                 active_player_->playerState);
        syncActivePlayer(state);
      }
      break;
    case homeassistant_media_player::RemotePlayerState::
        PowerOffRemotePlayerState:
    case homeassistant_media_player::RemotePlayerState::
        StoppedRemotePlayerState:
      ESP_LOGD(TAG, "Trying to sync active player - 4");
      syncActivePlayer(state);
  }
  publish_state(0);
}

void HomeAssistantMediaPlayerGroup::playSource(
    media_player_source::MediaPlayerSourceItem* source) {
  active_player_->clearSource();
  new_source_name = source->get_name();
  if (active_player_->get_player_type() ==
      homeassistant_media_player::RemotePlayerType::SpeakerRemotePlayerType) {
    HomeAssistantSpeakerMediaPlayer* activeSpeaker =
        static_cast<HomeAssistantSpeakerMediaPlayer*>(active_player_);
    activeSpeaker->playlist_title = source->get_name();
  }
  active_player_->playSource(source);
}

}  // namespace homeassistant_media_player
}  // namespace esphome
