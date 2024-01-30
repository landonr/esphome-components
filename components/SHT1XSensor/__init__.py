import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_HUMIDITY,
    CONF_ID,
    CONF_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
    UNIT_PERCENT,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_HUMIDITY,
)

MULTI_CONF = True

AUTO_LOAD = [ "binary_sensor", "sensor" ]
CONF_DATA_PIN = "data_pin"
CONF_CLOCK_PIN = "clock_pin"

sht1xsensor_ns = cg.esphome_ns.namespace('sht1xsensor')
SHT1XSensor = sht1xsensor_ns.class_('SHT1XSensor', cg.Component)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(SHT1XSensor),
            cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(
                unit_of_measurement=UNIT_PERCENT,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_HUMIDITY,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Required(CONF_DATA_PIN): cv.int_,
            cv.Required(CONF_CLOCK_PIN): cv.int_
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    cg.add_library("SHT1x-ESP", None)

    cg.add(var.set_data(config[CONF_DATA_PIN]))
    cg.add(var.set_clock(config[CONF_CLOCK_PIN]))

    if CONF_HUMIDITY in config:
        sens = await sensor.new_sensor(config[CONF_HUMIDITY])
        cg.add(var.set_humidity_sensor(sens))

    if CONF_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATURE])
        cg.add(var.set_temperature_sensor(sens))
