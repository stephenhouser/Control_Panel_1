#include "matrix_keyboard.h"
#include "esphome/core/log.h"

namespace esphome {
namespace matrix_keyboard {

static const char *const TAG = "matrix_keyboard";

void MatrixKeyboard::setup() {
  // Setup rows for output
  for (auto *row : this->rows_) {
    if (has_diodes_) {
      // put in tri-state mode when we have diodes
      row->pin_mode(gpio::FLAG_INPUT);
    } else {
      // regular output mode when no diodes
      row->pin_mode(gpio::FLAG_OUTPUT);
    }

    // this is needed to get the ouput pins working!
    row->setup();
  }

  // Setup columns for input, with pullups
  for (auto *col : this->columns_) {
    col->pin_mode(gpio::FLAG_INPUT | gpio::FLAG_PULLUP);
  }

  // Initilaize the current state of the keys
  int total_keys = this->columns_.size() * this->rows_.size();
  this->keys_.assign(total_keys, 0);

  FastLED.addLeds<WS2812, 5, RGB>(this->leds_, this->num_leds_);  // GRB ordering is typical
  FastLED.clear();
  FastLED.setBrightness(16);
  FastLED.show();
}

// returns led next to toggle switch
int led_for_key(int key) {
  //  keymap: "615B234C789A"
  int key_led_map[] = { 8, 9, 13, 1, 10, 11, 12, 1, 7, 6, 5, 4 };
  return key_led_map[key % 13];
}

void MatrixKeyboard::loop() {
  uint32_t now = millis();

  // keeps copy of last keyboard state so we can check for changes
  static std::vector<bool> last_keys;
  last_keys = this->keys_;
  // std::copy(this->keys_.begin(), this->keys_.end(), last_keys.begin());

  // scan the keyboard into this->keys_
  int key_pos = 0;
  for (auto *row : this->rows_) {
    if (has_diodes_) {
      row->pin_mode(gpio::FLAG_OUTPUT);
    }
    row->digital_write(false);

    for (auto *col : this->columns_) {
      auto state = !col->digital_read();
      this->keys_[key_pos++] = state;
    }

    if (has_diodes_) {
      row->pin_mode(gpio::FLAG_INPUT);
    }
  }

  // compare to last state and send changes
  if (this->keys_ != last_keys) {
    // update key and button listeners
    for (int k = 0; k < last_keys.size(); k++) {
      if (last_keys[k] != this->keys_[k]) {
        auto row = k / this->columns_.size();
        auto col = k % this->columns_.size();

        if (last_keys[k] == false) {  // pressed
          ESP_LOGD(TAG,"button_pressed(%d, %d)", col, row);

          for (auto &listener : this->listeners_) {
            listener->button_pressed(col, row);
          }

          int led = led_for_key(k);
          ESP_LOGD(TAG, "key=%d led=%d", k, led);
          this->leds_[led_for_key(k)] = CRGB::Red;
          FastLED.show();

          if (!this->keymap_.empty()) {
            auto keycode = this->keymap_.at(k);
            ESP_LOGD(TAG,"key_pressed(%c)", keycode);

            for (auto &listener : this->listeners_) {
              listener->key_pressed(keycode);
            }
            
            // send to key provider on press
            this->send_key_(keycode);
          }
        } else {                      // released
          ESP_LOGD(TAG,"button_released(%d, %d)", col, row);

          for (auto &listener : this->listeners_) {
            listener->button_released(col, row);
          }

          int led = led_for_key(k);
          ESP_LOGD(TAG, "key=%d led=%d", k, led);
          this->leds_[led_for_key(k)] = CRGB::Black;
          FastLED.show();

          if (!this->keymap_.empty()) {
            auto keycode = this->keymap_.at(k);
            ESP_LOGD(TAG,"key_released(%c)", keycode);

            for (auto &listener : this->listeners_) {
              listener->key_released(keycode);
            }
          }
        }
      }
    }
  }
}

void MatrixKeyboard::dump_config() {
  ESP_LOGCONFIG(TAG, "Matrix Keyboard:");
  ESP_LOGCONFIG(TAG, " Rows:");
  for (auto &pin : this->rows_) {
    LOG_PIN("  Pin: ", pin);
  }

  ESP_LOGCONFIG(TAG, " Cols:");
  for (auto &pin : this->columns_) {
    LOG_PIN("  Pin: ", pin);
  }
}

void MatrixKeyboard::register_listener(MatrixKeyboardListener *listener) {
  this->listeners_.push_back(listener); 
}

}  // namespace matrix_keyboard
}  // namespace esphome
