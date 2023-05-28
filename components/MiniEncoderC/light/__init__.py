import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, light
from esphome.const import (
    CONF_ID,  
    CONF_OUTPUT_ID,
)
from esphome.components.light import LightState

MULTI_CONF = False

AUTO_LOAD = [ "light" ]

DEPENDENCIES = ['i2c']

miniencoderc_ns = cg.esphome_ns.namespace('miniencoderc')
MiniEncoderCLightOutput = miniencoderc_ns.class_('MiniEncoderCLightOutput', light.LightOutput, cg.Component)
CONF_LIGHT = "light"

CONFIG_SCHEMA = light.RGB_LIGHT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(LightState),
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(MiniEncoderCLightOutput)
    }
).extend(i2c.i2c_device_schema(0x42))

async def to_code(config):
    # var = cg.new_Pvariable(config[CONF_ID])
    output = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await i2c.register_i2c_device(output, config)
    await light.register_light(output, config)
    # cg.add(var.set_light(output))
    # lightState = await cg.get_variable(config[CONF_ID])
    # cg.add(lightState.set_internal(True))