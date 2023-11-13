import esphome.codegen as cg
from esphome import automation
import esphome.config_validation as cv
from esphome.components import media_player, homeassistant_component, media_player_source
from esphome.const import CONF_ENTITY_ID, CONF_NAME, CONF_ID, CONF_INTERNAL, CONF_DISABLED_BY_DEFAULT, CONF_TRIGGER_ID

homeassistant_media_player_ns = cg.esphome_ns.namespace("homeassistant_media_player")

AUTO_LOAD = ['media_player', 'homeassistant_component', 'media_player_source', 'json']

MediaPlayerCommand = homeassistant_media_player_ns.class_("MediaPlayerCommand")
HomeAssistantBaseMediaPlayer = homeassistant_media_player_ns.class_("HomeAssistantBaseMediaPlayer", media_player.MediaPlayer, cg.Component)
HomeAssistantSpeakerMediaPlayer = homeassistant_media_player_ns.class_("HomeAssistantSpeakerMediaPlayer", media_player.MediaPlayer, cg.Component)
HomeAssistantTVMediaPlayer = homeassistant_media_player_ns.class_("HomeAssistantTVMediaPlayer", media_player.MediaPlayer, cg.Component)
HomeAssistantTVRokuMediaPlayer = homeassistant_media_player_ns.class_("HomeAssistantTVRokuMediaPlayer", HomeAssistantTVMediaPlayer, cg.Component)
HomeAssistantTVKodiMediaPlayer = homeassistant_media_player_ns.class_("HomeAssistantTVKodiMediaPlayer", media_player.MediaPlayer, cg.Component)
HomeAssistantTVSamsungMediaPlayer = homeassistant_media_player_ns.class_("HomeAssistantTVSamsungMediaPlayer", media_player.MediaPlayer, cg.Component)
HomeAssistantTVAndroidMediaPlayer = homeassistant_media_player_ns.class_("HomeAssistantTVAndroidMediaPlayer", media_player.MediaPlayer, cg.Component)

CONF_SPEAKER = "speaker"
CONF_TV = "tv"
CONF_ROKU = "roku"
CONF_KODI = "kodi"
CONF_SAMSUNG = "samsung"
CONF_ANDROID_TV = "android_tv"
CONF_SOUNDBAR = "soundbar"
CONF_SOURCES = "sources"
CONF_COMMAND = "command"
CONF_COMMANDS = "commands"


ServiceCalledTrigger = homeassistant_media_player_ns.class_(
    "ServiceCalledTrigger", automation.Trigger.template()
)

MEDIA_PLAYER_COMMAND_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(MediaPlayerCommand),
        cv.Required(CONF_COMMAND): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(
                    ServiceCalledTrigger
                ),
            }
        ),
        cv.Required(CONF_NAME): cv.string,
    }
)

MEDIA_PLAYER_COMMON_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_ENTITY_ID): cv.entity_id,
        cv.Required(CONF_NAME): cv.string,
        cv.Optional(CONF_INTERNAL, default=True): cv.boolean,
        cv.Optional(CONF_DISABLED_BY_DEFAULT, default=True): cv.boolean,
        cv.Optional(CONF_SOURCES): cv.All(
            cv.ensure_list(media_player_source.SOURCE_REFERENCE_SCHEMA), cv.Length(min=1)
        ),
        cv.Optional(CONF_COMMANDS): cv.All(
            cv.ensure_list(MEDIA_PLAYER_COMMAND_SCHEMA), cv.Length(min=1)
        ),
    }
).extend(homeassistant_component.COMPONENT_CONFIG_SCHEMA)

TV_CONFIG_SCHEMA = MEDIA_PLAYER_COMMON_SCHEMA.extend(
    {
        cv.Optional(CONF_SOUNDBAR): cv.Schema(
            {
                cv.Optional(CONF_SPEAKER): cv.use_id(HomeAssistantSpeakerMediaPlayer)
            },
            cv.has_exactly_one_key(CONF_SPEAKER),
        ),
    },
)

CONFIG_SCHEMA = cv.typed_schema(
    {
        CONF_SPEAKER: MEDIA_PLAYER_COMMON_SCHEMA.extend(
            {
                cv.GenerateID(CONF_ID): cv.declare_id(HomeAssistantSpeakerMediaPlayer),
            }
        ),
        CONF_TV: TV_CONFIG_SCHEMA.extend(
            {
                cv.GenerateID(CONF_ID): cv.declare_id(HomeAssistantTVMediaPlayer),
            }
        ),
        CONF_ROKU: TV_CONFIG_SCHEMA.extend(
            {
                cv.GenerateID(CONF_ID): cv.declare_id(HomeAssistantTVRokuMediaPlayer),
            }
        ),
        CONF_KODI: TV_CONFIG_SCHEMA.extend(
            {
                cv.GenerateID(CONF_ID): cv.declare_id(HomeAssistantTVKodiMediaPlayer),
            }
        ),
        CONF_SAMSUNG: TV_CONFIG_SCHEMA.extend(
            {
                cv.GenerateID(CONF_ID): cv.declare_id(HomeAssistantTVSamsungMediaPlayer),
            }
        ),
        CONF_ANDROID_TV: TV_CONFIG_SCHEMA.extend(
            {
                cv.GenerateID(CONF_ID): cv.declare_id(HomeAssistantTVAndroidMediaPlayer),
            }
        ),
    },
    lower=True,
)

async def to_code(config):
    cg.add_define("USE_MEDIA_PLAYER")
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await media_player.register_media_player(var, config)
    homeassistant_component.base_to_code(var, config)

    if CONF_SOUNDBAR in config:
        if CONF_SPEAKER in config[CONF_SOUNDBAR]:
            soundbar = await cg.get_variable(config[CONF_SOUNDBAR][CONF_SPEAKER])
            cg.add(soundbar.set_tv(var))
            cg.add(var.set_soundbar(soundbar))

    if CONF_SOURCES in config:
        for conf in config.get(CONF_SOURCES, []):
            new_source = await cg.get_variable(conf[CONF_ID])
            cg.add(var.register_source(new_source))

    if CONF_COMMANDS in config:
        for conf in config.get(CONF_COMMANDS, []):
            service = cg.new_Pvariable(conf[CONF_ID])
            cg.add(service.set_name(conf[CONF_NAME]))
            
            for command in conf.get(CONF_COMMAND, []):
                trigger = cg.new_Pvariable(command[CONF_TRIGGER_ID], service)
                await automation.build_automation(trigger, [], command)
            cg.add(var.register_custom_command(service))

# Automation

from esphome import automation
from esphome.automation import maybe_simple_id

UpAction = homeassistant_media_player_ns.class_(
    "UpAction", automation.Action, cg.Parented.template(HomeAssistantTVMediaPlayer)
)
DownAction = homeassistant_media_player_ns.class_(
    "DownAction", automation.Action, cg.Parented.template(HomeAssistantTVMediaPlayer)
)
LeftAction = homeassistant_media_player_ns.class_(
    "LeftAction", automation.Action, cg.Parented.template(HomeAssistantTVMediaPlayer)
)
RightAction = homeassistant_media_player_ns.class_(
    "RightAction", automation.Action, cg.Parented.template(HomeAssistantTVMediaPlayer)
)
SelectAction = homeassistant_media_player_ns.class_(
    "SelectAction", automation.Action, cg.Parented.template(HomeAssistantTVMediaPlayer)
)
BackAction = homeassistant_media_player_ns.class_(
    "BackAction", automation.Action, cg.Parented.template(HomeAssistantTVMediaPlayer)
)
HomeAction = homeassistant_media_player_ns.class_(
    "HomeAction", automation.Action, cg.Parented.template(HomeAssistantTVMediaPlayer)
)
PowerAction = homeassistant_media_player_ns.class_(
    "PowerAction", automation.Action, cg.Parented.template(HomeAssistantTVMediaPlayer)
)

MEDIA_PLAYER_TV_ACTION_SCHEMA = maybe_simple_id({cv.GenerateID(): cv.use_id(HomeAssistantTVMediaPlayer)})

@automation.register_action("media_player.up", UpAction, MEDIA_PLAYER_TV_ACTION_SCHEMA)
@automation.register_action("media_player.down", DownAction, MEDIA_PLAYER_TV_ACTION_SCHEMA)
@automation.register_action("media_player.left", LeftAction, MEDIA_PLAYER_TV_ACTION_SCHEMA)
@automation.register_action("media_player.right", RightAction, MEDIA_PLAYER_TV_ACTION_SCHEMA)
@automation.register_action("media_player.select", SelectAction, MEDIA_PLAYER_TV_ACTION_SCHEMA)
@automation.register_action("media_player.back", BackAction, MEDIA_PLAYER_TV_ACTION_SCHEMA)
@automation.register_action("media_player.home", HomeAction, MEDIA_PLAYER_TV_ACTION_SCHEMA)
@automation.register_action("media_player.power", PowerAction, MEDIA_PLAYER_TV_ACTION_SCHEMA)

async def media_player_tv_action(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])
    return var