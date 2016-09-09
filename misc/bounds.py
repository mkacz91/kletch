import fresnel
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.collections as mc
import scipy as sp
import math
import cmath

def jacobian_det(k0, k1, s):
    ds = cmath.rect(1, s * (k0 + 0.5 * k1 * s))
    dk1 = 0.5j * fresnel.eval_int_m2(k0, k1, s)
    result = ds.real * dk1.imag - ds.imag * dk1.real
    #print(result)
    return result

def charge(k0, k1, dk1, s, subdivs, ps):
    while subdivs >= 0:
        if jacobian_det(k0, k1, s) <= 0:
            dk1 /= 2
            k1 -= dk1
            subdivs -= 1
        else:
            ps.append(fresnel.eval_int(k0, k1, s))
            k1 += dk1

def main():
    c = fresnel.circle(1j, 1)


    ss = np.linspace(0, 20, 200)
    ps = []
    #ls = []
    for s in ss:
        k1s = np.linspace(-1, 1, 100)
        dk1 = 0.005
        charge(1, 0, dk1, s, 4, ps)
        charge(1, -dk1, -dk1, s, 4, ps)
    ps = np.array(ps)
    #lc = mc.LineCollection(ls)

    #t = linspace(0, 1.5 * math.pi, 100)
    plt.plot(c.real, c.imag, '-')
    plt.scatter(ps.real, ps.imag, c=None, marker='.')
    #plt.axes().add_collection(lc);
    plt.axes().set_aspect('equal', 'datalim')
    plt.margins(0.1, 0.1)
    plt.show()

if __name__ == "__main__":
    main()
