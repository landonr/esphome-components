# ESPHome Home ASsistant Media Player Integration

This integration provides support for various Home Assistant media player entities through ESPHome. It includes base media player types as well as specialized media players for speakers, TVs, and specific TV platforms like Roku, Kodi, Samsung, and Android TV.

## Features

- Media player components that connect to Home Assistant entities.
- Control media players from ESPHome remotes and automations.
- Configure media player sources and commands.
- Define custom actions for media players to access from menus.

## Installation

1. Add the media player component configuration to your `esphome` project.
```yaml
# example component installation
external_components:
  - source:
      type: git
      url: https://github.com/landonr/esphome-components
      ref: main
    refresh: 0s
    components: [
      homeassistant_media_player, # required for Home Assistant media player integration
      media_player_source, # required for media player sources
      media_player_source_sonos, # include if you have Sonos media player sources
      media_player_source_spotify, # include if you have Spotify media player sources
      media_player_source_custom # include if you have custom media player sources
    ]
```

## Configuration

The integration supports various media player types:

- **Speaker**: Represents a Home Assistant media player entity that is a speaker.
- **TV**: Represents a Home Assistant media player entity that is a TV.
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

#### Generic (or Sonos) Speaker Configuration:
```yaml
media_player:
  platform: homeassistant_media_player
  name: Sonos Beam
  entity_id: "media_player.beam"
  id: media_player_beam
  type: speaker

  # optional, include one or more sources
  sources:
    - id: sonos
      type: sonos
    - id: spotify
      type: spotify
  commands:
    name: "group all"
    command:
      - homeassistant.service:
          service: script.sonos_group_all
```

#### Spotify Speaker Configuration:
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

#### Roku TV Configuration:
```yaml
media_player:
  platform: homeassistant_media_player
  type: roku # roku, kodi, samsung, android_tv currently supported
  name: Roku TV
  entity_id: "media_player.55_tcl_roku_tv"
  id: media_player_tv

  # optional
  soundbar:
    speaker: media_player_beam
```

#### Kodi TV Configuration:
```yaml
media_player:
  platform: homeassistant_media_player
  type: kodi
  name: Kodi Desktop
  entity_id: "media_player.kodi_desktop"
  id: media_player_kodi
```

#### Samsung TV Configuration:
```yaml
media_player:
  platform: homeassistant_media_player
  type: samsumg
  id: my_samsung_tv
  name: "Living Room Samsung TV"
  entity_id: media_player.living_room_samsung_tv
```

#### Android TV Configuration:
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
