import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import fan, homeassistant_component
from esphome.const import CONF_ENTITY_ID, CONF_NAME, CONF_ID, CONF_INTERNAL, CONF_MAX_VALUE, CONF_MIN_VALUE, CONF_STEP

homeassistant_fan_ns = cg.esphome_ns.namespace("homeassistant_fan")

AUTO_LOAD = ['fan', 'homeassistant_component']

HomeAssistantFan = homeassistant_fan_ns.class_("HomeAssistantFan", fan.Fan, cg.Component, cg.EntityBase)

CONFIG_SCHEMA = fan.FAN_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(HomeAssistantFan),
        cv.Required(CONF_ENTITY_ID): cv.entity_id,
        cv.Required(CONF_NAME): cv.string,
        cv.Optional(CONF_INTERNAL, default=True): cv.boolean,
    }
).extend(homeassistant_component.COMPONENT_CONFIG_SCHEMA)

async def to_code(config):
    cg.add_build_flag("-DUSE_API_NUMBER")
    var = cg.new_Pvariable(config[CONF_ID])
    
    await cg.register_component(var, config)
    await fan.register_fan(var, config)
    homeassistant_component.base_to_code(var, config)
    return var