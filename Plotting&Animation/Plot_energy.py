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
t, E = np.genfromtxt("/home/wout/Documents/Astro_sim/Project/Tests&Systems/Plummer_100/Energy_error100.txt", unpack = True)

y = np.zeros((6))

for i in range(6):
    y[i] = 0.0000001 * (0.1 ** i)

plt.semilogy(t,E)
plt.title("Total energy for 100 particles")
plt.xlabel("t (s)")
plt.ylabel("Relative energy error")
plt.yticks(y)
plt.show()
