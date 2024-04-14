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
    - **Roku**: Represents a Home Assistant Roku TV media player.
    - **Kodi**: Represents a Home Assistant Kodi TV media player.
    - **Samsung**: Represents a Home Assistant Samsung TV media player.
    - **Android TV**: Represents a Home Assistant Android TV media player.

### Common Schema

- **`entity_id`**: (Required) The entity ID of the media player in Home Assistant.
- **`name`**: (Required) The name of the media player.
- **`internal`**: (Optional, default: `true`) Whether the media player is internal.
- **`disabled_by_default`**: (Optional, default: `true`) Whether the media player is disabled by default.
- **`sources`**: (Optional) A list of media player sources.
- **`commands`**: (Optional) A list of media player commands.

### Specific Media Player Configurations

1. **Speaker Configuration**:
    ```yaml
    speaker:
      id: my_speaker
      name: "Living Room Speaker"
      entity_id: media_player.living_room_speaker
      sources:
        - id: my_source
      commands:
        - id: my_command
          name: "My Command"
    ```

2. **TV Configuration**:
    ```yaml
    tv:
      id: my_tv
      name: "Living Room TV"
      entity_id: media_player.living_room_tv
      sources:
        - id: my_source
      commands:
        - id: my_command
          name: "My Command"
      soundbar:
        speaker: my_speaker
    ```

3. **Roku TV Configuration**:
    ```yaml
    roku:
      id: my_roku_tv
      name: "Living Room Roku TV"
      entity_id: media_player.living_room_roku_tv
      sources:
        - id: my_source
      commands:
        - id: my_command
          name: "My Command"
    ```

4. **Kodi TV Configuration**:
    ```yaml
    kodi:
      id: my_kodi_tv
      name: "Living Room Kodi TV"
      entity_id: media_player.living_room_kodi_tv
      sources:
        - id: my_source
      commands:
        - id: my_command
          name: "My Command"
    ```

5. **Samsung TV Configuration**:
    ```yaml
    samsung:
      id: my_samsung_tv
      name: "Living Room Samsung TV"
      entity_id: media_player.living_room_samsung_tv
      sources:
        - id: my_source
      commands:
        - id: my_command
          name: "My Command"
    ```

6. **Android TV Configuration**:
    ```yaml
    android_tv:
      id: my_android_tv
      name: "Living Room Android TV"
      entity_id: media_player.living_room_android_tv
      sources:
        - id: my_source
      commands:
        - id: my_command
          name: "My Command"
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
