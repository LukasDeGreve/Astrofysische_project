import math as m
import numpy as np
import scipy.special
import matplotlib
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import csv
from matplotlib import animation
from random import randint


#Loading in data
t1, E1 = np.genfromtxt("/home/wout/Documents/Astro_sim/Project/Fatsoenlijke_data/3_body/Energy_3_body_h=0.010000e=0.000000.txt", unpack = True)
t2, E2 = np.genfromtxt("/home/wout/Documents/Astro_sim/Project/Fatsoenlijke_data/3_body/Energy_3_body_h=0.001000e=0.000000.txt", unpack = True)
t3, E3 = np.genfromtxt("/home/wout/Documents/Astro_sim/Project/Fatsoenlijke_data/3_body/Energy_3_body_h=0.000100e=0.000000.txt", unpack = True)

y = np.zeros((6))

for i in range(6):
    y[i] = 10 ** (-4-i)

plt.semilogy(t1,E1,label=r'$h = 0.01$')
plt.semilogy(t2,E2,label=r'$h = 0.001$')
plt.semilogy(t3,E3,label=r'$h = 0.0001$')
plt.title("Three body system: Relative energy error")
plt.xlabel(r"$t$ (program units)")
plt.ylabel(r"$\delta E/E$")
plt.yticks(y)
plt.grid()
plt.legend(loc='upper right', bbox_to_anchor=(0.98,0.85))
plt.show()
