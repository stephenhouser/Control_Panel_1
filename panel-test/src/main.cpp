#include <Arduino.h>
/// @file    Blink.ino
/// @brief   Blink the first LED of an LED strip
/// @example Blink.ino

#include <FastLED.h>
#include <RotaryEncoder.h>


// How many leds in your strip?
#define NUM_LEDS 13
#define LED_DATA_PIN 5
CRGB leds[NUM_LEDS];

int led_counter = 0;
CRGB led_colors[] = { CRGB::Red, CRGB::Yellow, CRGB::Blue, CRGB::Orange, CRGB::Green, CRGB::Black };

int led_map[] = { 8, 9, 10, 11, 12,
                  7, 6,  5,  4,  3,
                  0, 0, 
                  1, 2 };


#define KEY_COLS 4
#define KEY_ROWS 3
#define ALL_KEYS ((KEY_COLS * KEY_ROWS) + 2)

int key_col_pins[] = { 25, 26, 27, 33 };
int key_row_pins[] = { 12, 13, 14 };
#define ENCODER_BUTTON_PIN 19
#define CALL_BUTTON_PIN 18

// int keys[KEY_COLS][KEY_ROWS];
int keys[ALL_KEYS];
  // maps keys to switch number                      // encoder and call
int keymap[] = {  5,  0,  4, 10, 1,
                  2,  3, 11,  6, 7, 
                  8,  9, 
                 13, 12 };
int decoded_keys[ALL_KEYS];


#define RE_PINA 23
#define RE_PINB 21
#define RE_PINC 22
RotaryEncoder encoder(RE_PINA, RE_PINC, RotaryEncoder::LatchMode::TWO03);
int encoder_value = 0;


void led_setup() {
  FastLED.addLeds<WS2812, LED_DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(8);
  for (int l = 0; l < NUM_LEDS; l++) {
    leds[l] = CRGB::Black;
  }
}

void re_loop() {
  pinMode(RE_PINB, OUTPUT);
  digitalWrite(RE_PINB, LOW);

  encoder.tick();
  encoder_value = encoder.getPosition();
  if (encoder_value < 1) {
    encoder_value = 1;
  }
}

void led_loop() {
  for (int k = 0; k < ALL_KEYS; k++) {
    if (decoded_keys[k]) {
      leds[led_map[k]] = CRGB::Black;
    } else {
      leds[led_map[k]] = CRGB::Red;
    }
  }

  if (!decoded_keys[10]) {
      leds[led_map[10]] = CRGB::Green;
  }

  FastLED.show();
}

// void led_loop() {
//   for (int l = 0; l < NUM_LEDS; l++) {
//     int color = (led_counter + l) % 6;
//     leds[l] = led_colors[color];
//   }

//   led_counter = (led_counter + 1) % 6; 
//   FastLED.show();
// }

void key_setup() {
  for (int r = 0; r < KEY_ROWS; r++) {
    pinMode(key_row_pins[r], INPUT);
    for (int c = 0; c < KEY_COLS; c++) {
      pinMode(key_col_pins[c], INPUT_PULLUP);
    }
  }

  pinMode(ENCODER_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CALL_BUTTON_PIN, INPUT_PULLUP);
}

void key_scan() {
  for (int r = 0; r < KEY_ROWS; r++) {
    int row_pin = key_row_pins[r];
    pinMode(row_pin, OUTPUT);
    digitalWrite(row_pin, LOW);

    for (int c = 0; c < KEY_COLS; c++) {
      int col_pin = key_col_pins[c];
      pinMode(col_pin, INPUT_PULLUP);
      int state = digitalRead(col_pin);
      keys[(r * KEY_COLS) + c] = state;
      decoded_keys[keymap[(r * KEY_COLS) + c]] = state;
    }

    digitalWrite(row_pin, HIGH);
    pinMode(row_pin, INPUT);

    decoded_keys[keymap[KEY_ROWS * KEY_COLS]] = digitalRead(ENCODER_BUTTON_PIN);
    decoded_keys[keymap[KEY_ROWS * KEY_COLS + 1]] = digitalRead(CALL_BUTTON_PIN);
  }  
}

void key_show() {
  for (int k = 0; k < ALL_KEYS; k++) {
      Serial.printf("%d", decoded_keys[k]);
      if (k % 4 == 3) {
        Serial.printf("  ");
      } else {
        Serial.printf(" ");
      }
  }

  Serial.println();
}


void setup() { 
  Serial.begin(115200);
  Serial.println("Starting up...");
  key_setup();
  led_setup();
}

void loop() { 
  key_scan();
  // key_show();

  led_loop();
  re_loop();

  int bright = encoder_value % 256;
  if (bright != FastLED.getBrightness()) {
    Serial.printf("Brightness = %d\n", bright);
    FastLED.setBrightness(bright);
  }

  // delay(100);
}
