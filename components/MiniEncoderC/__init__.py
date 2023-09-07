import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor, i2c, sensor
from esphome.const import (
    CONF_ID, 
    CONF_TRIGGER_ID
)
from esphome.components.MiniEncoderC.light import MiniEncoderCLightOutput
from esphome import automation
from esphome.components.light import LightState

MULTI_CONF = True

AUTO_LOAD = [ "binary_sensor", "sensor" ]

DEPENDENCIES = ['i2c']

miniencoderc_ns = cg.esphome_ns.namespace('miniencoderc')
MiniEncoderC = miniencoderc_ns.class_('MiniEncoderC', i2c.I2CDevice, cg.Component)
CONF_ON_CLOCKWISE = "on_clockwise"
CONF_ON_ANTICLOCKWISE = "on_anticlockwise"
CONF_ENCODER_FILTER = "encoder_filter"
CONF_BUTTON = "button"
CONF_ENCODER = "encoder"

MiniEncoderCClockwiseTrigger = miniencoderc_ns.class_(
    "MiniEncoderCClockwiseTrigger", automation.Trigger
)
MiniEncoderCAnticlockwiseTrigger = miniencoderc_ns.class_(
    "MiniEncoderCAnticlockwiseTrigger", automation.Trigger
)

ENCODER_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_ON_CLOCKWISE): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(
                    MiniEncoderCClockwiseTrigger
                ),
            }
        ),
        cv.Optional(CONF_ON_ANTICLOCKWISE): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(
                    MiniEncoderCAnticlockwiseTrigger
                ),
            }
        ),
        cv.Optional(CONF_ENCODER_FILTER, default=1): cv.int_range(min=1, max=100),
    }
).extend(sensor.sensor_schema())

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(MiniEncoderC),
        cv.Optional(CONF_ENCODER): ENCODER_SCHEMA,
        cv.Optional(CONF_BUTTON): binary_sensor.binary_sensor_schema(),
    }
).extend(i2c.i2c_device_schema(0x42))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    if CONF_ENCODER in config:
        sens = await sensor.new_sensor(config[CONF_ENCODER])
        cg.add(var.set_encoder(sens))

        encoderConfig = config[CONF_ENCODER]

        if CONF_ENCODER_FILTER in encoderConfig:
            cg.add(var.set_encoder_filter(encoderConfig[CONF_ENCODER_FILTER]))

        for conf in encoderConfig.get(CONF_ON_CLOCKWISE, []):
            trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
            await automation.build_automation(trigger, [], conf)
        for conf in encoderConfig.get(CONF_ON_ANTICLOCKWISE, []):
            trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
            await automation.build_automation(trigger, [], conf)

    if CONF_BUTTON in config:
        button = await binary_sensor.new_binary_sensor(config[CONF_BUTTON])
        cg.add(var.set_button(button))