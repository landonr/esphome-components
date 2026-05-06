import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light
from esphome.const import CONF_ENTITY_ID, CONF_OUTPUT_ID, CONF_NAME, CONF_ID, CONF_INTERNAL

from .. import COMPONENT_CONFIG_SCHEMA, base_to_code

homeassistant_light_ns = cg.esphome_ns.namespace("homeassistant_light")

AUTO_LOAD = ['light']

HomeAssistantLight = homeassistant_light_ns.class_("HomeAssistantLight", light.LightOutput, cg.Component, cg.EntityBase)
HomeAssistantLightState = homeassistant_light_ns.class_("HomeAssistantLightState", light.LightState)

LIGHT_SCHEMA = light.LIGHT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(HomeAssistantLightState),
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(HomeAssistantLight),
    }
)

CONFIG_SCHEMA = COMPONENT_CONFIG_SCHEMA.extend(LIGHT_SCHEMA)

async def to_code(config):
    cg.add_build_flag("-DUSE_API_LIGHT")
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await cg.register_component(var, config)
    await light.register_light(var, config)
    base_to_code(var, config)
