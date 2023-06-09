import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number, homeassistant_component
from esphome.const import CONF_ENTITY_ID, CONF_NAME, CONF_ID, CONF_INTERNAL, CONF_MAX_VALUE, CONF_MIN_VALUE, CONF_STEP

homeassistant_number_ns = cg.esphome_ns.namespace("homeassistant_number")

AUTO_LOAD = ['number', 'homeassistant_component']

HomeAssistantNumber = homeassistant_number_ns.class_("HomeAssistantNumber", number.Number, cg.Component, cg.EntityBase)

CONFIG_SCHEMA = number.NUMBER_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(HomeAssistantNumber),
        cv.Required(CONF_ENTITY_ID): cv.entity_id,
        cv.Required(CONF_NAME): cv.string,
        cv.Optional(CONF_INTERNAL, default=True): cv.boolean,
        cv.Required(CONF_MAX_VALUE): cv.float_,
        cv.Required(CONF_MIN_VALUE): cv.float_,
        cv.Required(CONF_STEP): cv.positive_float,
    }
).extend(homeassistant_component.COMPONENT_CONFIG_SCHEMA)

async def to_code(config):
    cg.add_build_flag("-DUSE_API_NUMBER")
    var = cg.new_Pvariable(config[CONF_ID])
    
    await cg.register_component(var, config)
    await number.register_number(
        var,
        config,
        min_value=config[CONF_MIN_VALUE],
        max_value=config[CONF_MAX_VALUE],
        step=config[CONF_STEP],
    )
    homeassistant_component.base_to_code(var, config)
    return var