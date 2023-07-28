import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ID, CONF_KEY
from .. import MatrixKeyboard, matrix_keyboard_ns, CONF_KEYBOARD_ID

CONF_ROW = "row"
CONF_COL = "col"

DEPENDENCIES = ["matrix_keyboard"]

MatrixKeyboardBinarySensor = matrix_keyboard_ns.class_(
    "MatrixKeyboardBinarySensor", binary_sensor.BinarySensor
)


def check_button(obj):
    if CONF_ROW in obj or CONF_COL in obj:
        if CONF_KEY in obj:
            raise cv.Invalid("You can't provide both a key and a position")
        if CONF_ROW not in obj:
            raise cv.Invalid("Missing row")
        if CONF_COL not in obj:
            raise cv.Invalid("Missing col")
    elif CONF_KEY not in obj:
        raise cv.Invalid("Missing key or position")
    elif len(obj[CONF_KEY]) != 1:
        raise cv.Invalid("Key must be one character")
    return obj


CONFIG_SCHEMA = cv.All(
    binary_sensor.binary_sensor_schema(MatrixKeyboardBinarySensor).extend(
        {
            cv.GenerateID(CONF_KEYBOARD_ID): cv.use_id(MatrixKeyboard),
            cv.Optional(CONF_ROW): cv.int_,
            cv.Optional(CONF_COL): cv.int_,
            cv.Optional(CONF_KEY): cv.string,
        }
    ),
    check_button,
)


async def to_code(config):
    if CONF_KEY in config:
        var = cg.new_Pvariable(config[CONF_ID], config[CONF_KEY][0])
    else:
        var = cg.new_Pvariable(config[CONF_ID], config[CONF_ROW], config[CONF_COL])
    await binary_sensor.register_binary_sensor(var, config)
    matrix_keyboard = await cg.get_variable(config[CONF_KEYBOARD_ID])
    cg.add(matrix_keyboard.register_listener(var))
