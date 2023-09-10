import hassapi as hass

# panel: dev_control_panel

class ControlPanel(hass.Hass):
    def initialize(self):
        panel = self.args['panel']

        for switch_config in self.args['switches']:
            self.initialize_switch(switch_config, panel)

    def initialize_switch(self, switch_config, panel):
        switch_config = self.parse_switch_config(switch_config)
        switch_id = switch_config['id']
        
        if 'switch' not in switch_config:
            switch_config['switch'] = f'binary_sensor.{panel}_switch_{switch_id}'

        if 'light' not in switch_config:
            switch_config['light'] =  f'light.{panel}_led_{switch_id}'

        switch = switch_config['switch']
        self.listen_state(self.switch_on, switch, new="on", **switch_config)
        self.listen_state(self.switch_off, switch, new="off", **switch_config)

        if 'target' in switch_config:
            target = switch_config['target']
            self.listen_state(self.target_on, target, new="on", **switch_config)
            self.listen_state(self.target_off, target, new="off", **switch_config)

            # initialize lights...
            target_state = self.get_state(target)
            if target_state == 'on':
                self.target_on(switch, None, None, None, switch_config)
            else:
                self.target_off(switch, None, None, None, switch_config)

    def parse_switch_config(self, switch_config):
        if 'id' not in switch_config:
            switch_id = list(switch_config.keys())[0]

            defaults = self.args['defaults']
            target = switch_config[switch_id]
            if type(target) is str:
                config = { **defaults, 'target': target }
            else:
                config = { **defaults, **target }

            config['id'] = switch_id
            return config

        return switch_config

    def switch_on(self, switch, attribute, old, new, cb_args):
        light = cb_args['light']
        target = cb_args['target'] if 'target' in cb_args else None
        target_state = self.get_state(target) if target else None

        self.log(f'{switch}=ON, light={light}, target={target}')

        # if target on change to 'color' else to 'sync_color'
        if target_state == 'on' or target is None:
            self.turn_on(light, **cb_args['color_on'])
        else:
            self.turn_on(light, **cb_args['color_sync'])
            self.turn_on(target)

    def switch_off(self, switch, attribute, old, new, cb_args):
        light = cb_args['light']
        target = cb_args['target'] if 'target' in cb_args else None
        target_state = self.get_state(target) if target else None
        
        self.log(f'{switch}=off, light={light}, target={target}')

        # if target on change to 'sync_color' else off
        if target_state == 'on':
            self.turn_on(light, **cb_args['color_sync'])
            self.turn_off(target)
        else:
            self.turn_off(light)

    def target_on(self, switch, attribute, old, new, cb_args):
        light = cb_args['light']
        switch = cb_args['switch']
        switch_state = self.get_state(switch)

        # if switch off change to 'sync_color' else to 'color'
        if switch_state == 'off':
            self.turn_on(light, **cb_args['color_sync'])
        else:
            self.turn_on(light, **cb_args['color_on'])

    def target_off(self, switch, attribute, old, new, cb_args):
        light = cb_args['light']
        switch = cb_args['switch']
        switch_state = self.get_state(switch)

        # if switch on change to 'sync_color' else off
        if switch_state == 'on':
            self.turn_on(light, **cb_args['color_sync'])
        else:
            self.turn_off(light)

