import simpy

class Car(object):
    def __init__(self, env):
        self.env = env

        self.action = self.env.process(self.run())

    def run(self):
        for i in range(10):
            print('Start parking and charging at %d' % self.env.now)
            charge_duration = 5
            try:
                yield self.env.process(self.charge(charge_duration))

                print('Start driving at %d' % self.env.now)
                driving_time = 2
                yield self.env.timeout(driving_time)

            except simpy.Interrupt:
                print('Interrupt')
                return            

    def charge(self, duration):
        yield self.env.timeout(duration)

def driver(env, car):
    yield env.timeout(13)
    car.action.interrupt()
    print('Gotham is under my feet')

env = simpy.Environment()
car = Car(env)

env.process(driver(env, car))
env.run()
