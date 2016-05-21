import numpy as np
import scipy.integrate as integrate
import matplotlib.pyplot as plt
import cmath
import math
from math import exp, pow

def exec1(s, n):
    g = np.zeros(n, dtype=complex)
    etois = cmath.rect(1, s)
    stok = 1;
    g[0] = etois - 1
    for k in range(1, n):
        stok *= s
        g[k] = stok * etois - k * g[k - 1]
    jfact = 1
    for j in range(0, n):
        g[j] /= jfact
        jfact *= (j + 1)

    plt.plot(g.real, g.imag, '-')
    plt.show()

def exec2(n):
    a = np.zeros(n)
    kfact = 1
    for k in range(0, n):
        a[k] = integrate.quad(lambda t: pow(t, 2 * k) * exp(t), 0, 1)[0] / kfact
        kfact *= (k + 1)
    return a

def exec3(n):
    a = np.zeros(n)
    kfact = 1
    a[0] = math.e - 1
    for k in range(1, n):
        kfact *= k
        a[k] = math.e / kfact + 2 * (2 * k - 1) * a[k - 1] - 2 * math.e * k / kfact
        #a[k] = math.e * (1 - 2 * k) / kfact + 2 * (2 * k - 1) * a[k - 1]
    return a

def exec4(n, m):
    a = np.zeros(n)
    ifactinv = 1
    for i in range(0, n):
        kfactinv = 1
        ai = 0
        for k in range(0, m):
            ai += kfactinv / (2 * i + k + 1)
            kfactinv /= (k + 1)
        a[i] = ai * ifactinv
        ifactinv /= (i + 1)
    return a

if __name__ == "__main__":
    n = 20
    a2 = exec2(n)
    a3 = exec3(n)
    a4 = exec4(n, 5)
    #plt.plot(np.linspace(0, n - 1, n), a2, 'o')
    #plt.plot(np.linspace(0.2, n - 1 + 0.2, n), a3, '+')
    #plt.plot(np.linspace(0.4, n - 1 + 0.4, n), a4, 'x')
    plt.plot(np.linspace(0, n - 1, n), a2 - a4, '-')
    plt.show()
