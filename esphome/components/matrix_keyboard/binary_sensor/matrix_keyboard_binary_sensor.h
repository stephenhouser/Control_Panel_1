#pragma once

#include "esphome/components/matrix_keyboard/matrix_keyboard.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace matrix_keyboard {

class MatrixKeyboardBinarySensor : public MatrixKeyboardListener, public binary_sensor::BinarySensor {
 public:
  MatrixKeyboardBinarySensor(uint8_t key) : has_key_(true), key_(key){};
  MatrixKeyboardBinarySensor(const char *key) : has_key_(true), key_((uint8_t) key[0]){};
  MatrixKeyboardBinarySensor(int col, int row) : has_key_(false), row_(row), col_(col){};

  void key_pressed(uint8_t key) override {
    if (!this->has_key_)
      return;
    if (key == this->key_)
      this->publish_state(true);
  }

  void key_released(uint8_t key) override {
    if (!this->has_key_)
      return;
    if (key == this->key_)
      this->publish_state(false);
  }

  void button_pressed(int col, int row) override {
    if (this->has_key_)
      return;
    if ((row == this->row_) && (col == this->col_))
      this->publish_state(true);
  }

  void button_released(int col, int row) override {
    if (this->has_key_)
      return;
    if ((row == this->row_) && (col == this->col_))
      this->publish_state(false);
  }

 protected:
  bool has_key_;
  uint8_t key_;
  int row_;
  int col_;
};

}  // namespace matrix_keyboard
}  // namespace esphome
