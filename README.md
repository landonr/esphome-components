# esphome-components

<h3 align="center">
ESPHome components to control Home Assistant stuff
used by <a href="https://github.com/landonr/homeThing">homeThing! ❤️</a>
</h3>
<h5 align="center"><a href="https://homeThing.io">homeThing.io</a></h5>

## Media Player
### [ESPHome](https://esphome.io/components/media_player/index.html) | [Home Assistant Component](https://github.com/landonr/esphome-components/tree/main/components/homeassistant_media_player/media_player)
* Speaker features:
	* Control playback (play/pause, next, previous, volume)
	* Change source
	* Control speaker grouping
* Screen menu control
	* Remote commands differ between devices
	* Supports Roku, Samsung and Kodi currently
* Screen soundbar support
	* A speaker set as a screens soundbar will show up as grouped to the TV when playing media

```yaml
media_player:
  - platform: homeassistant_media_player
    name: Beam
    entity_id: "media_player.beam"
    id: media_player_beam
    type: speaker
    sources:
      - id: sonos
        type: sonos
      - id: spotty
        type: spotify
      - id: fav_playlists
        type: custom
```

## Custom media sources
### [Home Assistant Component](https://github.com/landonr/esphome-components/tree/main/components/media_player_source)
Sources are lists of inputs or media that are available to media players
* Speaker sources
	* Spotify playlists (uses the [Spotcast Integration](https://github.com/fondberg/spotcast))
	* Sonos favorites
	* Custom playlists
* Screen sources
	* Custom YouTube video lists

```yaml
media_player_source_sonos:
  id: sonos

media_player_source_spotify:
  id: spotty

media_player_source_custom:
  - id: fav_playlists
    name: spotify playlists
    sources:
      name: idk what this is
      media_id: spotify:playlist:2uzbATYxs9V8YQi5lf89WG
      media_type: playlist
  - id: youtube_videos
    name: boiler room sets
    sources:
      - name: Four Tet x Fred again.. x Skrillex - Coachella 2023
        media_id: dFfC92iBxNw
        media_type: source
```

## Lights
### [ESPHome](https://esphome.io/components/light/index.html) | [Home Assistant Component](https://github.com/landonr/esphome-components/tree/main/components/homeassistant_light_group/light)
* Toggle, Brightness, Temperature and RGB color control

```yaml
light:
  - platform: homeassistant_light_group
    id: light_rgb_lights
    entity_id: light.rgb_lights
    name: Color Lights 
```

## Switches
### [ESPHome](https://esphome.io/components/switch/index.html) | [Home Assistant Component](https://github.com/landonr/esphome-components/tree/main/components/homeassistant_switch_group)
* Switch control (on/off)

```yaml
switch:
  - platform: homeassistant_switch_group
    entity_id: "switch.oven_fan"
    name: "Oven Fan"
    id: oven_fan_switch
```

------------


#### There is some menu code leftover that I need to move into a different repo. Each type of component has it's own Group component that is passed to homeThingThere is some menu code leftover that I need to move into a different repo. Each type of component has it's own Group component that is passed to homeThing
