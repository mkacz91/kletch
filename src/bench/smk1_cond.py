from subprocess import Popen, PIPE
import numpy as np
import matplotlib.pyplot as plt
from scipy import signal

def smooth(x, radius):
    x = np.r_[x[radius+1:0:-1],x,x[-1:-radius:-1]]
    w = np.hanning(2 * radius + 1)
    return np.convolve(w / w.sum(), x, mode='valid')

process = Popen(["../../build/msvcfb/Debug/kletch_bench", "smk1_cond"], stdout=PIPE)
(output, err) = process.communicate()
values = np.array(list(map(float, output.decode().split()))).reshape((-1, 2))
xs = values[:,0]
ys = values[:,1]
plt.xscale('log')
plt.yscale('log')
plt.plot(xs, ys)
plt.plot(xs, smooth(ys, 50))
#plt.plot(xs, signal.savgol_filter(ys, 201, 1), color='red')
plt.show()
