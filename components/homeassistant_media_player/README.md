# HomeAssistantMediaPlayerGroup Component
The HomeAssistantMediaPlayerGroup component for ESPHome allows you to group multiple Home Assistant media players into a single entity. This is particularly useful for creating a unified menu for controlling playback across multiple media players.

## Base Configuration
The HomeAssistantMediaPlayerGroup component has several base configuration options. By default, appropriate values will be chosen for all of these options, but you can override them if you want to.

```yaml
# Example media player group configuration
homeassistant_media_player:
  id: media_group_component
  media_players:
    - id: media_player_beam
      type: speaker
    - id: media_player_tv
      type: roku
  finished_loading:
    id: media_players_loaded_sensor
  active_player:
    id: active_player
    name: "${friendly_name} Active Player"
```

Configuration Variables
 - **id** *(Optional, string)*: Manually specify the ID for code generation. At least one of id and name must be specified.
 - **name** *(Optional, string)*: The name for the media player group. At least one of id and name must be specified.
 - **media_players** *(Required, list)*: List of media player entities to include in the media player group. Can be a local or [Home Assistant media player entity](https://github.com/landonr/esphome-components/tree/main/components/homeassistant_media_player/media_player)
 - **finished_loading** *(Optional)*: Binary sensor indicating when the media player group has finished loading. Used for Boot menu
 - **active_player** *(Optional)*: Text sensor providing the entity ID about the currently active media player in the group. Useful for creating a single image entity for the currently active media player. [Now Playing image info](https://github.com/landonr/homeThing/blob/main/common/nowPlayingImage.yaml)