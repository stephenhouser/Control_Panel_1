#pragma once

#include "esphome/components/key_provider/key_provider.h"
#include "esphome/core/component.h"
#include "esphome/components/light/addressable_light.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"
#include <cstdlib>
#include <utility>
#include "FastLED.h"

namespace esphome {
namespace led_matrix_keyboard {

class LEDMatrixKeyboardListener {
 public:
  virtual void button_pressed(int col, int row){};
  virtual void button_released(int col, int row){};
  virtual void key_pressed(uint8_t key){};
  virtual void key_released(uint8_t key){};
};

class LEDMatrixKeyboard : public key_provider::KeyProvider, public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  void set_columns(std::vector<GPIOPin *> pins) { columns_ = std::move(pins); };
  void set_rows(std::vector<GPIOPin *> pins) { rows_ = std::move(pins); };
  void set_key_map(std::string key_map) { key_map_ = std::move(key_map); };
  void set_debounce_time(int debounce_time) { debounce_time_ = debounce_time; };
  void set_has_diodes(int has_diodes) { has_diodes_ = has_diodes; };
  
  void set_light(light::LightState *light_state) { 
    //from addressable_light_dispaly
    light_state_ = light_state; 
    light_ = static_cast<light::AddressableLight *>(light_state->get_output());
    }
  void set_light_map(std::string light_map) { light_map_ = std::move(light_map); }

  void register_listener(LEDMatrixKeyboardListener *listener);

 protected:
  std::vector<GPIOPin *> rows_;
  std::vector<GPIOPin *> columns_;
  std::vector<bool> keys_;
  std::string key_map_;
  int debounce_time_ = 0;
  bool has_diodes_{false};

  light::LightState *light_state_;
  light::AddressableLight *light_;

  std::string light_map_;

  std::vector<LEDMatrixKeyboardListener *> listeners_{};
};

}  // namespace led_matrix_keyboard
}  // namespace esphome
