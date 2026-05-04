import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import cover
from esphome.const import CONF_ENTITY_ID, CONF_NAME, CONF_ID, CONF_INTERNAL

from .. import COMPONENT_CONFIG_SCHEMA, base_to_code

homeassistant_cover_ns = cg.esphome_ns.namespace("homeassistant_cover")

AUTO_LOAD = ['cover']

HomeAssistantCover = homeassistant_cover_ns.class_("HomeAssistantCover", cover.Cover, cg.Component, cg.EntityBase)

_BASE_COVER_SCHEMA = getattr(cover, "COVER_SCHEMA", None)
if _BASE_COVER_SCHEMA is None:
    _BASE_COVER_SCHEMA = cover.cover_schema(HomeAssistantCover)
else:
    _BASE_COVER_SCHEMA = _BASE_COVER_SCHEMA.extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(HomeAssistantCover),
            cv.Required(CONF_NAME): cv.string,
            cv.Optional(CONF_INTERNAL, default=True): cv.boolean,
        }
    )

CONFIG_SCHEMA = _BASE_COVER_SCHEMA.extend(
    {
        cv.Required(CONF_ENTITY_ID): cv.entity_id,
    }
).extend(COMPONENT_CONFIG_SCHEMA)

async def to_code(config):
    cg.add_build_flag("-DUSE_API_COVER")
    var = cg.new_Pvariable(config[CONF_ID])
    
    await cg.register_component(var, config)
    await cover.register_cover(var, config)
    base_to_code(var, config)
    return var
