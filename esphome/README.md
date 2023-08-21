# ESPHome Control Panel Configuration

This directory holds the configuration for the control panel with [ESPHome](https://esphome.io) and [HomeAssistant](https://www.home-assistant.io).

- **components**: ESPHome custom component for the `matrix_keyboard`. Will need to be copied into the ESPHome directory to be included into the YAML configurations.

- **blueprints**: contains blueprintes for ESPHome to configure each toggle switch to be connected to an LED on the panel and a device within HomeAssistant. Each toggle and it's associated LED should be connected to a `control-panel-switch`

- **common**: contans some common includes to be used in ESPHome. These are optional. If not used, the `include` statements will need to be removed from the YAML files.

- **Example Congifurations**:
  - `living-room-cp.yaml`: Living Room Control Panel (production)
  - `dev-control-panel.yaml`: Development Control Panel (development, Office)
  - `dev-sound.yaml`: Experiment with i2s sound
