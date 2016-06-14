import fresnel
import numpy as np
import matplotlib.pyplot as plt

k0s = [0, 0.1, 1, 2, -0.1, -1]
k1s = [0.01, 0.5, 1, 2, -1]
ss = [0, 0.5, 1, 2] # negative s is not allowed (although it would probably work)
k0n = len(k0s)
k1n = len(k1s)
sn = len(ss)
n = k0n * k1n * sn
xs = np.zeros(n)
ys = np.zeros(n)
i = 0
print('real values[][5] =\n{')
for k0 in k0s:
    for k1 in k1s:
        for s in ss:
            f = fresnel.eval_int(k0, k1, s)
            xs[i] = f.real
            ys[i] = f.imag
            i += 1
            print('    {{rl({0}), rl({1}), rl({2}), rl({3}), rl({4})}},'
                .format(k0, k1, s, f.real, f.imag))
print('    {rl(-1000), rl(0), rl(0), rl(0), rl(0)}\n};')
plt.plot(xs, ys, 'o')
plt.show()
