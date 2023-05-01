import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, binary_sensor
from esphome.const import (
    CONF_ID, 
    CONF_BATTERY_LEVEL, 
    DEVICE_CLASS_VOLTAGE, 
    ICON_PERCENT,
    UNIT_PERCENT,
    STATE_CLASS_MEASUREMENT,
    DEVICE_CLASS_BATTERY
)

# media_player_source_ns = cg.esphome_ns.namespace("media_player_source")

# MediaPlayerSourceBase = media_player_source_ns.class_("MediaPlayerSourceBase", cg.Component)

CONF_CHARGER_CONNECTED = "charger_connected"
CONF_CHARGE_FULL = "charge_full"

# CustomSourceComponent = cg.esphome_ns.namespace("media_player_source_custom").class_("CustomSourceComponent", cg.Component)
# SonosSourceComponent = cg.esphome_ns.namespace("media_player_source_sonos").class_("SonosSourceComponent", cg.Component)
# SpotifySourceComponent = cg.esphome_ns.namespace("media_player_source_spotify").class_("SpotifySourceComponent", cg.Component)
# SOURCE_REFERENCE_SCHEMA = cv.typed_schema(
#     {
#         CONF_SONOS: cv.Schema(
#             {
#                 cv.GenerateID(CONF_ID): cv.use_id(SonosSourceComponent),
#             }
#         ),
#         CONF_SPOTIFY: cv.Schema(
#             {
#                 cv.GenerateID(CONF_ID): cv.use_id(SpotifySourceComponent),
#             }
#         ),
#         CONF_CUSTOM: cv.Schema(
#             {
#                 cv.GenerateID(CONF_ID): cv.use_id(CustomSourceComponent),
#             }
#         ),
#     }
# )

BASE_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(IP5306),
        cv.Optional(CONF_BATTERY_LEVEL): sensor.sensor_schema(
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_BATTERY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_CHARGER_CONNECTED): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_CHARGE_FULL): binary_sensor.binary_sensor_schema(),
    }
).extend(cv.COMPONENT_SCHEMA)

def new_source_base(config):
    var = cg.new_Pvariable(config[CONF_ID])
    
    if CONF_ENTITY_ID in config:
        cg.add(var.set_entity_id(config[CONF_ENTITY_ID]))
    if CONF_NAME in config:
        cg.add(var.set_name(config[CONF_NAME]))
    return var