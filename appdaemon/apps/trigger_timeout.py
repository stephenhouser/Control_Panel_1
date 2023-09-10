import hassapi as hass

class TriggerTimeout(hass.Hass):
    def initialize(self):
        sensors = self.args['sensor']
        if type(sensors) is str:
            sensors = [sensors]

        for sensor in sensors:
            self.listen_state(self.targets_on, sensor, new = "on")

    def targets_on(self, entity, attribute, old, new, cb_args):
        color = self.args['color']
        timeout = self.args['timeout']

        targets = self.args['target']
        if type(targets) is str:
            targets = [targets]

        for target in targets:
            self.log(f'Turn {target} {color} for {timeout}s')
            self.turn_on(target, color_name=color, effect='Pulse Red')

        self.run_in(self.targets_off, timeout)

    def targets_off(self, cb_args):
        targets = self.args['target']
        if type(targets) is str:
            targets = [targets]

        for target in targets:
            self.log(f'Turn off {target}')
            self.turn_off(target)
