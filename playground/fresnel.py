import numpy as np
from numpy import linspace
import matplotlib.pyplot as plt
from scipy import special
import math
from math import sqrt, copysign
import cmath

def fresnel_k0_abg(a, s):
    c0 = sqrt(abs(a) / math.pi)
    y, x = special.fresnel(c0 * s)
    return complex(x / c0, copysign(1, a) * y / c0)

def fresnel_k0_asm(a, s):
    h = complex(0, 0.5 * a)
    c = complex(1)
    d = s
    ss = s * s
    result = complex(s)
    for k in range(1, 3):
        c *= h / k
        d *= ss
        result += c * d / (2 * k + 1)
    return result

def fresnel0(s):
    im, re = special.fresnel(s);
    return complex(re, im);

def fresneln_k1_asm(n, t, f, e, a, s):
    h = complex(0, 0.5 * a)
    c = complex(1)
    result = f
    for k in range(1, 3):
        c *= h / k
        t *= s
        f = t * e * complex(n + 2, -s) - (n + 1) * (n + 2) * f
        t *= s
        n += 2
        result += c * f
    return result

def fresnel0_k1_asm(a, s):
    e = cmath.rect(1, s)
    return fresneln_k1_asm(0, 1, 1j - 1j * e, e, a, s)

fresnel0_k1_asm_v = np.vectorize(fresnel0_k1_asm)

def fresnel0_k1_abg(a, s):
    c0 = sqrt(abs(a) / math.pi)
    c1 = 1 / a
    s0 = c0 * (s + c1)
    s1 = c0 * c1
    result = (fresnel0(s0) - fresnel0(s1)) / c0;
    if (a < 0):
        result = result.conjugate()
    result *= cmath.rect(1, -0.5 * c1)
    return result

fresnel0_k1_abg_v = np.vectorize(fresnel0_k1_abg)

crect_v = np.vectorize(cmath.rect)
def circle(center, radius):
    return center + crect_v(radius, linspace(0, 2 * math.pi, 50))

def main():
    c = circle(1j, 1)
    t = linspace(0, 1.5 * math.pi, 100)
    a = -0.00000000000001
    bgf = fresnel0_k1_abg_v(a, t)
    smf = fresnel0_k1_asm_v(a, t)
    plt.plot(bgf.real, bgf.imag, '-')
    plt.plot(smf.real, smf.imag, '+')
    plt.plot(c.real, c.imag, '-')
    plt.xlim([-1.1,-0.9])
    plt.ylim([0.9, 1.1])
    plt.show()

if __name__ == "__main__":
    main()
