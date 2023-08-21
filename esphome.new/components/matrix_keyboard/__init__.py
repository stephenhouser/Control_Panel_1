import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import key_provider
from esphome.const import CONF_ID, CONF_PIN

CODEOWNERS = ["@stephenhouser"]

AUTO_LOAD = ["key_provider"]

MULTI_CONF = True

matrix_keyboard_ns = cg.esphome_ns.namespace("matrix_keyboard")
MatrixKeyboard = matrix_keyboard_ns.class_(
    "MatrixKeyboard", key_provider.KeyProvider, cg.Component
)

CONF_KEYBOARD_ID = "keyboard_id"
CONF_ROWS = "rows"
CONF_COLUMNS = "columns"
CONF_KEYMAP = "keymap"
CONF_DEBOUNCE_TIME = "debounce_time"
CONF_HAS_DIODES = "has_diodes"


def check_keymap(obj):
    if CONF_KEYMAP in obj:
        if len(obj[CONF_KEYMAP]) != len(obj[CONF_ROWS]) * len(obj[CONF_COLUMNS]):
            raise cv.Invalid(
                "The number of keys in the keymap must equal the number of buttons in rows and columns"
            )
    return obj


CONFIG_SCHEMA = cv.All(
    cv.COMPONENT_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(MatrixKeyboard),
            cv.Required(CONF_ROWS): cv.All(
                cv.ensure_list({cv.Required(CONF_PIN): pins.gpio_output_pin_schema}),
                cv.Length(min=1),
            ),
            cv.Required(CONF_COLUMNS): cv.All(
                cv.ensure_list({cv.Required(CONF_PIN): pins.gpio_input_pin_schema}),
                cv.Length(min=1),
            ),
            cv.Optional(CONF_KEYMAP): cv.string,
            cv.Optional(CONF_DEBOUNCE_TIME, default=1): cv.int_range(min=1, max=100),
            cv.Optional(CONF_HAS_DIODES): cv.boolean,
        }
    ),
    check_keymap,
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    row_pins = []
    for conf in config[CONF_ROWS]:
        pin = await cg.gpio_pin_expression(conf[CONF_PIN])
        row_pins.append(pin)
    cg.add(var.set_rows(row_pins))
    col_pins = []
    for conf in config[CONF_COLUMNS]:
        pin = await cg.gpio_pin_expression(conf[CONF_PIN])
        col_pins.append(pin)
    cg.add(var.set_columns(col_pins))
    if CONF_KEYMAP in config:
        cg.add(var.set_keymap(config[CONF_KEYMAP]))
    cg.add(var.set_debounce_time(config[CONF_DEBOUNCE_TIME]))
    if CONF_HAS_DIODES in config:
        cg.add(var.set_has_diodes(config[CONF_HAS_DIODES]))
