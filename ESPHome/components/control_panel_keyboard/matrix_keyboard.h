#pragma once

#include "esphome/components/key_provider/key_provider.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"
#include <cstdlib>
#include <utility>
#include "FastLED.h"

namespace esphome {
namespace matrix_keyboard {

class MatrixKeyboardListener {
 public:
  virtual void button_pressed(int col, int row){};
  virtual void button_released(int col, int row){};
  virtual void key_pressed(uint8_t key){};
  virtual void key_released(uint8_t key){};
};

class MatrixKeyboard : public key_provider::KeyProvider, public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  void set_columns(std::vector<GPIOPin *> pins) { columns_ = std::move(pins); };
  void set_rows(std::vector<GPIOPin *> pins) { rows_ = std::move(pins); };
  void set_keymap(std::string keymap) { keymap_ = std::move(keymap); };
  void set_debounce_time(int debounce_time) { debounce_time_ = debounce_time; };
  void set_has_diodes(int has_diodes) { has_diodes_ = has_diodes; };
  void set_leds(int pin, int num_leds) { led_pin_ = pin; num_leds_ = num_leds; }

  void register_listener(MatrixKeyboardListener *listener);

 protected:
  std::vector<GPIOPin *> rows_;
  std::vector<GPIOPin *> columns_;
  std::vector<bool> keys_;
  std::string keymap_;
  int debounce_time_ = 0;
  bool has_diodes_{false};
  int led_pin_ = 0;
  int num_leds_ = -1;
  CRGB leds_[20];

  std::vector<MatrixKeyboardListener *> listeners_{};
};

}  // namespace matrix_keyboard
}  // namespace esphome
