import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_NAME
from esphome.components import media_player_source

AUTO_LOAD = ["media_player_source"]

CONF_SOURCES = "sources"
CONF_MEDIA_ID = "media_id"
CONF_MEDIA_TYPE = "media_type"
CONF_APP_TYPE = "app"

media_player_source_custom_ns = cg.esphome_ns.namespace("media_player_source_custom")

CustomSourceComponent = media_player_source_custom_ns.class_("CustomSourceComponent", cg.Component)

media_player_source_ns = cg.esphome_ns.namespace("media_player_source")
MediaPlayerSourceType = media_player_source_ns.enum("MediaPlayerSourceType")
AppPlayerSourceType = media_player_source_ns.enum("AppPlayerSourceType")
MEDIA_TYPE_OPTIONS = {
    "music": MediaPlayerSourceType.MediaPlayerSourceTypeMusic,
    "favorite": MediaPlayerSourceType.MediaPlayerSourceTypeFavoriteItemID,
    "source": MediaPlayerSourceType.MediaPlayerSourceTypeSource,
    "playlist": MediaPlayerSourceType.MediaPlayerSourceTypePlaylist,
    "app": MediaPlayerSourceType.MediaPlayerSourceTypeApp,
}

CONF_NONE = "none"
CONF_YOUTUBE = "youtube"
CONF_SPOTIFY = "spotify"
CONF_NETFLIX = "netflix"
CONF_PLEX = "plex"
CONF_DISNEY_PLUS = "disney_plus"
CONF_HULU = "hulu"
CONF_AMAZON_PRIME = "amazon_prime"
CONF_HBO = "hbo"

SOURCE_APP = {
    CONF_NONE: AppPlayerSourceType.AppPlayerSourceTypeNone,
    CONF_YOUTUBE: AppPlayerSourceType.AppPlayerSourceTypeYouTube,
    CONF_SPOTIFY: AppPlayerSourceType.AppPlayerSourceTypeSpotify,
    CONF_NETFLIX: AppPlayerSourceType.AppPlayerSourceTypeNetflix,
    CONF_PLEX: AppPlayerSourceType.AppPlayerSourceTypePlex,
    CONF_DISNEY_PLUS: AppPlayerSourceType.AppPlayerSourceTypeDisneyPlus,
    CONF_HULU: AppPlayerSourceType.AppPlayerSourceTypeHulu,
    CONF_AMAZON_PRIME: AppPlayerSourceType.AppPlayerSourceTypePrimeVideo,
    CONF_HBO: AppPlayerSourceType.AppPlayerSourceTypeHBO,
}

CUSTOM_SOURCE_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_NAME): cv.string,
        cv.Required(CONF_MEDIA_ID): cv.string,
        cv.Required(CONF_MEDIA_TYPE): cv.enum(MEDIA_TYPE_OPTIONS, upper=False),
        cv.Optional(CONF_APP_TYPE, default=CONF_NONE): cv.enum(SOURCE_APP, upper=False),
    }
)

CONFIG_SCHEMA = cv.All(
    cv.ensure_list(
        media_player_source.BASE_SCHEMA.extend({
            cv.GenerateID(CONF_ID): cv.declare_id(CustomSourceComponent),
            cv.Optional(CONF_NAME, default="Custom"): cv.string,
            cv.Required(CONF_SOURCES): cv.All(
                cv.ensure_list(CUSTOM_SOURCE_SCHEMA), cv.Length(min=1)
            ),
        }), 
        cv.Length(min=1)
    )
)        

async def to_code(config):
    for custom_source in config:
        var = media_player_source.new_source_base(custom_source)
        await cg.register_component(var, custom_source)

        for conf in custom_source.get(CONF_SOURCES, []):
            new_name = conf[CONF_NAME]
            new_id = conf[CONF_MEDIA_ID]
            new_type = conf[CONF_MEDIA_TYPE]
            cg.add(var.add_source(new_name, new_id, new_type, conf[CONF_APP_TYPE]))