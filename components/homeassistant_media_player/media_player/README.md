# ESPHome Home ASsistant Media Player Integration

This integration provides support for various Home Assistant media player entities through ESPHome. It includes base media player types as well as specialized media players for speakers, TVs, and specific TV platforms like Roku, Kodi, Samsung, and Android TV.

## Features

- Define media player components that connect to Home Assistant entities.
- Control media players from ESPHome nodes.
- Configure media player sources and commands.
- Define media player actions for TV media players.

## Installation

1. Add the media player component configuration to your `esphome` project.
2. Configure the desired media players and their options according to the available schema.
3. Compile and flash the code to your ESPHome device.

## Configuration

The integration supports various media player types:

- **Speaker**: Represents a Home Assistant media player entity that is a speaker.
- **TV**: Represents a Home Assistant media player entity that is a TV.
- **TV Types**:
    - **roku**: Represents a Home Assistant Roku TV media player.
    - **kodi**: Represents a Home Assistant Kodi TV media player.
    - **samsung**: Represents a Home Assistant Samsung TV media player.
    - **android_tv**: Represents a Home Assistant Android TV media player.

### Common Schema

- **`entity_id`**: (Required) The entity ID of the media player in Home Assistant.
- **`name`**: (Required) The name of the media player.
- **`internal`**: (Optional, default: `true`) Whether the media player is internal.
- **`disabled_by_default`**: (Optional, default: `true`) Whether the media player is disabled by default.
- **`sources`**: (Optional) A list of media player sources.
- **`commands`**: (Optional) A list of media player commands.

### Specific Media Player Configurations

1. #### Speaker Configuration:
```yaml
media_player:
  platform: homeassistant_media_player
  name: Sonos Beam
  entity_id: "media_player.beam"
  id: media_player_beam
  type: speaker

  # optional
  sources:
    - id: sonos
      type: sonos
  commands:
    name: "group all"
    command:
      - homeassistant.service:
          service: script.sonos_group_all
```

2. #### Spotify Speaker Configuration:
```yaml
media_player:
  platform: homeassistant_media_player
  name: Spotify
  entity_id: "media_player.spotify_landorghini"
  id: media_player_spotify
  type: speaker

  # optional
  sources:
    - id: spotty
      type: spotify
```

2. #### TV Configuration:
```yaml
media_player:
  platform: homeassistant_media_player
  type: roku
  name: Roku TV
  entity_id: "media_player.55_tcl_roku_tv"
  id: media_player_tv

  # optional
  soundbar:
    speaker: media_player_beam
```

4. #### Kodi TV Configuration:
```yaml
media_player:
  platform: homeassistant_media_player
  type: kodi
  name: Kodi Desktop
  entity_id: "media_player.kodi_desktop"
  id: media_player_kodi
```

5. #### Samsung TV Configuration:
```yaml
media_player:
  platform: homeassistant_media_player
  type: samsumg
  id: my_samsung_tv
  name: "Living Room Samsung TV"
  entity_id: media_player.living_room_samsung_tv
```

6. #### Android TV Configuration:
```yaml
media_player:
  platform: homeassistant_media_player
  type: android_tv
  id: my_android_tv
  name: "Living Room Android TV"
  entity_id: media_player.living_room_android_tv
```

## Automation Actions

You can use the following actions for TV media players:

- `media_player.up`
- `media_player.down`
- `media_player.left`
- `media_player.right`
- `media_player.select`
- `media_player.back`
- `media_player.home`
- `media_player.power`

Refer to the [ESPHome Automation documentation](https://esphome.io/components/automation/index.html) for how to integrate these actions into your ESPHome node's automation.

## License

This project is licensed under the terms of the MIT License.

---
