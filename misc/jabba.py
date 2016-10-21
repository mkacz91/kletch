import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import scipy as sp
from scipy import integrate
from math import pi, sin, cos, sqrt
import cmath

def eval_fresnel(k0, k1, s):
    x, _ = integrate.quad(lambda t: cos(t * (k0 + 0.5 * k1 * t)), 0, s)
    y, _ = integrate.quad(lambda t: sin(t * (k0 + 0.5 * k1 * t)), 0, s)
    return (x, y)

def eval_int_m2(k0, k1, s):
    re, _ = integrate.quad(lambda t: t * t * cos(t * (k0 + 0.5 * k1 * t)), 0, s)
    im, _ = integrate.quad(lambda t: t * t * sin(t * (k0 + 0.5 * k1 * t)), 0, s)
    return complex(re, im)

def jacobian_det(k0, k1, s):
    ds = cmath.rect(1, s * (k0 + 0.5 * k1 * s))
    dk1 = 0.5j * eval_int_m2(k0, k1, s)
    result = ds.real * dk1.imag - ds.imag * dk1.real
    #print(result)
    return result

def eval_jacobian(k0, k1, s):
    f2x, _ = integrate.quad(lambda t: t * t * cos(t * (k0 + 0.5 * k1 * t)), 0, s);
    f2y, _ = integrate.quad(lambda t: t * t * sin(t * (k0 + 0.5 * k1 * t)), 0, s);
    j12 = cos(s * (k0 + 0.5 * k1 * s));
    j22 = sin(s * (k0 + 0.5 * k1 * s));
    return np.matrix([
        [-0.5 * f2y, j12],
        [ 0.5 * f2x, j22]
    ]);

def get_k1_range(k0, s, dth):
    c0 = k0 * s
    delta = c0 * (2 * dth - c0) + dth * dth;
    if delta <= 0:
        return (False, 0, 0)

    c1 = -c0 - dth
    sqrt_delta = sqrt(delta)
    ss = s * s

    b0 = (c1 - sqrt_delta) / ss
    b1 = (c1 + sqrt_delta) / ss
    if b0 > b1:
        b0, b1 = b1, b0

    b2 = 2 * (dth - c0) / ss

    return (True, b0, b1 if b2 * s < -k0 else b2)

def main():
    k0 = 1
    ds = 0.1
    #dk1 = 0.05
    dth = pi
    s = ds
    xs = []
    ys = []
    zs = []
    while True:
        b, k1_start, k1_end = get_k1_range(k0, s, dth)
        if not b:
            break
        for k1 in np.linspace(k1_start - 0.2, k1_end, 40):
            x, y = eval_fresnel(k0, k1, s)
            xs.append(x)
            ys.append(y)
            #J = eval_jacobian(k0, k1, s)
            #detJ = J[0, 0] * J[1, 1] - J[0, 1] * J[1, 0];
            zs.append(jacobian_det(k0, k1, s))
        s += ds
    minz = min(zs)
    maxz = max(zs)
    for i in range(0, zs.length):
        z = zs[i]
        if z


    plt.set_cmap('seismic')
    #fig = plt.figure()
    #ax = fig.add_subplot(111, projection='3d')
    plt.scatter(xs, ys, c=zs, marker='+')
    plt.axis('equal')
    plt.show()

main()
