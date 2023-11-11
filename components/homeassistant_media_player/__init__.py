import esphome.codegen as cg
from esphome.components import sensor
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_ENTITY_ID, CONF_NAME
from esphome.components.homeassistant_media_player.media_player import (
    homeassistant_media_player_ns, 
    HomeAssistantSpeakerMediaPlayer, 
    HomeAssistantTVMediaPlayer,
    HomeAssistantTVRokuMediaPlayer,
    HomeAssistantTVKodiMediaPlayer,
    HomeAssistantTVSamsungMediaPlayer,
    HomeAssistantTVAndroidMediaPlayer,
    CONF_SPEAKER,
    CONF_TV,
    CONF_ROKU,
    CONF_KODI,
    CONF_SAMSUNG,
    CONF_ANDROID_TV,
)
from esphome.components import sensor, binary_sensor

CONF_MEDIA_PLAYERS = "media_players"
CONF_FINISHED_LOADING = "finished_loading"
AUTO_LOAD = ["sensor"]
DEPENDENCIES = ["api", "sensor"]

HOMEASSISTANT_MEDIA_PLAYER_REFERENCE_SCHEMA = cv.typed_schema(
    {
        CONF_SPEAKER: cv.Schema(
            {
                cv.GenerateID(CONF_ID): cv.use_id(HomeAssistantSpeakerMediaPlayer),
            }
        ),
        CONF_TV: cv.Schema(
            {
                cv.GenerateID(CONF_ID): cv.use_id(HomeAssistantTVMediaPlayer),
            }
        ),
        CONF_ROKU: cv.Schema(
            {
                cv.GenerateID(CONF_ID): cv.use_id(HomeAssistantTVRokuMediaPlayer),
            }
        ),
        CONF_KODI: cv.Schema(
            {
                cv.GenerateID(CONF_ID): cv.use_id(HomeAssistantTVKodiMediaPlayer),
            }
        ),
        CONF_SAMSUNG: cv.Schema(
            {
                cv.GenerateID(CONF_ID): cv.use_id(HomeAssistantTVSamsungMediaPlayer),
            }
        ),
        CONF_ANDROID_TV: cv.Schema(
            {
                cv.GenerateID(CONF_ID): cv.use_id(HomeAssistantTVAndroidMediaPlayer),
            }
        ),
    },
    lower=True,
)

HomeAssistantMediaPlayerGroup = homeassistant_media_player_ns.class_(
    'HomeAssistantMediaPlayerGroup', cg.Component, sensor.Sensor
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(HomeAssistantMediaPlayerGroup),
        cv.Required(CONF_MEDIA_PLAYERS): cv.All(
            cv.ensure_list(HOMEASSISTANT_MEDIA_PLAYER_REFERENCE_SCHEMA), cv.Length(min=1)
        ),
        cv.Optional(CONF_FINISHED_LOADING): binary_sensor.binary_sensor_schema(),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    cg.add_build_flag("-DUSE_MEDIA_PLAYER_GROUP")
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    for conf in config.get(CONF_MEDIA_PLAYERS, []):
        new_media_player = await cg.get_variable(conf[CONF_ID])
        cg.add(var.register_media_player(new_media_player))

    if finished_loading := config.get(CONF_FINISHED_LOADING):
        binary_sens = await binary_sensor.new_binary_sensor(finished_loading)
        cg.add(var.set_finished_loading_sensor(binary_sens))

# Automation

from esphome import automation
from esphome.automation import maybe_simple_id

SelectNextMediaPlayerAction = homeassistant_media_player_ns.class_("SelectNextMediaPlayerAction", automation.Action)
MEDIA_PLAYER_GROUP_ACTION_SCHEMA = maybe_simple_id(
    {
        cv.GenerateID(CONF_ID): cv.use_id(HomeAssistantMediaPlayerGroup),
    }
)

@automation.register_action("media_player_group.select_next_player", SelectNextMediaPlayerAction, MEDIA_PLAYER_GROUP_ACTION_SCHEMA)
async def select_next_media_player_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)
