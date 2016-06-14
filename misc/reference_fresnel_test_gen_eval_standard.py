from scipy import special
import numpy as np
import matplotlib.pyplot as plt

n = 100
xs = np.zeros(n)
ys = np.zeros(n)
ss = np.linspace(0, 3, n)
print('real values[][3] =\n{')
for i in range(0, n):
    s = ss[i]
    y, x = special.fresnel(s)
    print('    {{rl({0}), rl({1}), rl({2})}},'.format(s, x, y))
    xs[i] = x
    ys[i] = y
print('    {rl(-1), rl(-1), rl(-1)}\n};')
plt.plot(xs, ys, '-')
plt.show()
