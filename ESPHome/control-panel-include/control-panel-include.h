#include "esphome.h"

class ControlPanel;

class ControlPanelLight : public Component, public LightOutput {
	private:
		ControlPanel *parent;
		int led_num;

	public:
		ControlPanelLight(ControlPanel *parent, int led_num) {			
			this->parent = parent;
			this->led_num = led_num;
		}

		void setup() override {
		}

		LightTraits get_traits() override {			
    		// return the traits this light supports
    		auto traits = LightTraits();
    		traits.set_supported_color_modes({ColorMode::RGB, ColorMode::BRIGHTNESS});
    		return traits;			
		}

		void write_state(LightState *state) override {
			this->parent->write_state(state, led_num);
		}
};

class ControlPanel : public Component {
	private:
		std::vector<int> row_pins;
		std::vector<int> col_pins;
		int led_pin;
		int num_leds;
		uint8_t *leds;

    public:
		std::vector<BinarySensor *> keys;
		std::vector<LightOutput *> lights;

  	ControlPanel(std::vector<int> row_pins,	std::vector<int> col_pins, int led_pin, int num_leds) {
		this->row_pins = row_pins;
		this->col_pins = col_pins;

		int num_keys = row_pins.size() * col_pins.size();
		for (int i = 0; i < num_keys; i++) {
			this->keys.push_back(new BinarySensor());
		}

		this->led_pin = led_pin;
		this->num_leds = num_leds;
		for (int i = 0; i < num_leds; i++) {
			this->lights.push_back(new ControlPanelLight(this, i));
		}

		this->leds = new uint8_t[this->num_leds];
		FastLED.addLeds<ws2811, this->led_pin>(this->leds, this->num_leds);
	}

	void write_state(LightState *state, int led_num) {
		ESP_LOGD("CP", "write_state(%d)", led_num);
	}

	void setup() override {
	}

	void loop() override {
    }
};

