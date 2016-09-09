import numpy as np
from numpy import linspace
import matplotlib.pyplot as plt
from scipy import special, integrate
import math
from math import sqrt, copysign, hypot, sin, cos
import cmath

def eval_standard(s):
    im, re = special.fresnel(s);
    return complex(re, im);

def eval_int(k0, k1, s):
    re, _ = integrate.quad(lambda t: cos(t * (k0 + 0.5 * k1 * t)), 0, s)
    im, _ = integrate.quad(lambda t: sin(t * (k0 + 0.5 * k1 * t)), 0, s)
    return complex(re, im)

def eval_int_m2(k0, k1, s):
    re, _ = integrate.quad(lambda t: t * t * cos(t * (k0 + 0.5 * k1 * t)), 0, s)
    im, _ = integrate.quad(lambda t: t * t * sin(t * (k0 + 0.5 * k1 * t)), 0, s)
    return complex(re, im)

def eval_bgk1(k0, k1, s):
    c0 = sqrt(abs(k1 / math.pi))
    c1 = k0 / k1
    c2 = -0.5 * k0 * c1
    s0 = c0 * c1
    s1 = c0 * (s + c1)
    result = (eval_standard(s1) - eval_standard(s0)) / c0
    if k1 < 0:
        result.imag = -result.imag
    result *= cmath.rect(1, c2)
    return result

def eval_smk1(k0, k1, s, n = 0, th = 1e-5):
    f = complex(0, 0)
    a = complex(0, 0.5 * k1 * s * s)
    b = complex(0, k0 * s)
    m = 0
    dm = n + 1
    am = complex(1, 0)
    while True:
        g = complex(0, 0)
        l = 0
        bl = complex(1, 0)
        dl = dm
        while True:
            dg = bl / dl
            g += dg
            if abs(dg.real) <= th * abs(g.real) and abs(dg.imag) <= th * abs(g.imag):
                break
            l += 1
            bl *= b / l
            dl += 1
        df = am * g
        f += df
        if abs(df.real) <= th * abs(f.real) and abs(df.imag) <= th * abs(f.imag):
            break
        m += 1
        am *= a / m
        dm += 2
    while n >= 0:
        f *= s
        n -= 1
    return f

eval_standard_v = np.vectorize(eval_standard)
eval_int_v = np.vectorize(eval_int)
eval_bgk1_v = np.vectorize(eval_bgk1)
eval_smk1_v = np.vectorize(eval_smk1)
crect_v = np.vectorize(cmath.rect)

def circle(center, radius):
    return center + crect_v(radius, linspace(0, 2 * math.pi, 50))

def main():
    c = circle(1j, 1)
    t = linspace(0, 1.5 * math.pi, 100)
    k1 = 1e-16
    smfs = eval_smk1_v(1, k1, t, 0, 1e-10)
    bgfs = eval_bgk1_v(1, k1, t)
    infs = eval_int_v(1, k1, t)
    plt.plot(smfs.real, smfs.imag, 'b-')
    plt.plot(bgfs.real, bgfs.imag, 'r-')
    plt.plot(infs.real, infs.imag, '+')
    plt.plot(c.real, c.imag, '-')
    plt.xlim([-1.000001,-0.999999])
    plt.ylim([0.999999, 1.000001])
    plt.show()

if __name__ == "__main__":
    main()
