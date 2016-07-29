import genutil
from scipy import special
import numpy as np
import matplotlib.pyplot as plt

def gen(s0, s1, n):
    values = []
    for s in np.linspace(s0, s1, n):
        im, re = special.fresnel(s)
        values.append([s, re, im])
    return values

def gen():
    return gen(0, 5, 100)

if __name__ == "__main__":
    values = gen()
    genutil.print_values(values)
    ss, xs, ys = genutil.get_columns(values)
    plt.plot(xs, ys, '-')
    plt.show()
