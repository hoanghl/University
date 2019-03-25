import math
import simpy
import numpy

class Job:
    def __init__(self, arrTime, servTime):
        self.arrTime  = arrTime
        self.servTime = servTime


class Terminal:
    def __init__(self, env, server):
        self.server = server
        env.process(self.running(env))

    def running(self, env):
        while True:
            terminal_thinking = self.thinking_time(numpy.random.rand())
            yield env.timeout(terminal_thinking)

            terminal_serving  = self.serving_time(numpy.random.rand())
            self.server.Jobs.append(Job(env.n))


    def thinking_time(x, beta = 1/15):
        return beta * math.exp(-x * beta)

    def serving_time(x, beta = 1/0.5):
        return beta * math.exp(-x * beta)


def main():
    ready_queue = []
    i = 0.01
    while (i < 3):
        tmp = (i)
        print("%5.5f" % tmp)
        ready_queue.append(tmp)
        i += 0.01
    
    time = 0.2
    while (len(ready_queue) != 0):
        head = ready_queue.pop(0)
        print('Head = %5.5f' % head)
        if (head - time > 0):
            ready_queue.append(head - time)
    

if __name__ == "__main__":
    main()

