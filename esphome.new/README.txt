


Versions

- `control-panel-prototype-1.yaml` original control panel prototype

- `control-panel-matrix.yaml` with `matrix-keypad` compoment in components

- `control-panel-include.yaml` with local .h files to create sensors

- `control-panel.yaml` working copy









# https://esphome.io/components/sensor/custom.html
class MyCustomSensor : public PollingComponent {
 public:
  Adafruit_BMP085 bmp;
  Sensor *temperature_sensor = new Sensor();
  Sensor *pressure_sensor = new Sensor();

  MyCustomSensor() : PollingComponent(15000) { }

  void setup() override {
    bmp.begin();
  }

  void update() override {
    // This is the actual sensor reading logic.
    float temperature = bmp.readTemperature();
    temperature_sensor->publish_state(temperature);

    int pressure = bmp.readPressure();
    pressure_sensor->publish_state(pressure / 100.0);
  }
};

sensor:
- platform: custom
  lambda: |-
    auto my_sensor = new MyCustomSensor();
    App.register_component(my_sensor);
    return {my_sensor->temperature_sensor, my_sensor->pressure_sensor};

  sensors:
  - name: "RE 1"
    accuracy_decimals: 1

# https://esphome.io/components/binary_sensor/custom.html
binary_sensor:
  - platform: custom
    lambda: |-
      auto my_custom_sensor = new MyCustomBinarySensor();
      App.register_component(my_custom_sensor);
      return {my_custom_sensor};

    binary_sensors:
      - name: "SW 1"
      - name: "SW 2"
      - name: "SW 3"

# https://esphome.io/components/light/custom.html
light:
- platform: custom
  lambda: |-
    auto light_out = new MyCustomLightOutput();
    App.register_component(light_out);
    return {light_out};

  lights:
    - name: "SW 1"
    - name: "SW 2"
    - name: "SW 3"
