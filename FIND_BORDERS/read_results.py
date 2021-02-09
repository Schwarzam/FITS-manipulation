import os
import numpy as np
import matplotlib.pyplot as plt

comm = os.popen('./main').read()

spl = comm.split('\n')
for item in spl:
    if item.startswith("X_array: "):
        X = np.array(item.strip('X_array: ').split(' ')).astype(int)
    if item.startswith("Y_array: "):
        Y = np.array(item.strip('Y_array: ').split(' ')).astype(int)


plt.scatter(X, Y)

Xleft = X.min()
Xright = X.max()
Ytop = Y.min()
Ybottom = Y.max()

