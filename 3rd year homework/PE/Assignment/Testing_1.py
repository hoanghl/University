import math
import numpy

def serving_time(x, beta = 1/0.5):
    return beta * math.exp(-x * beta)

for i in range(10):
    print("%f" %serving_time(numpy.random.rand()))
