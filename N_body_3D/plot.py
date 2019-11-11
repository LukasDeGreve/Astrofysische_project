import math as m
import numpy as np
import scipy.special
import matplotlib.pyplot as plt
from matplotlib import animation
from mpl_toolkits.mplot3d import Axes3D
import csv

#Loading in data
p, x, y, z = np.genfromtxt("test.txt", unpack = True)
#print(p)
#print(x)
#print(y)
N = 3
iterations = 100000;
x1 = np.zeros((iterations))
y1 = np.zeros((iterations))
x2 = np.zeros((iterations))
y2 = np.zeros((iterations))
x3 = np.zeros((iterations))
y3 = np.zeros((iterations))
z1 = np.zeros((iterations))
z2 = np.zeros((iterations))
z3 = np.zeros((iterations))

#print(x.size)
i = 0
j = 0
k = 1
l = 2
while i < iterations:
   #print(i)
    x1[i] = x[j]
    y1[i] = y[j]
    x2[i] = x[k]
    y2[i] = y[k]
    x3[i] = x[l]
    y3[i] = y[l]
    z1[i] = z[i]
    z2[i] = z[j]
    z3[i] = z[k]
    j+=N
    k+=N
    l+=N
    i+=1
#print(x1)
#print(y1)


#P3.3.1
#Plotting dSigma as a function of Theta for one energy
xmin = -4.5
xmax = 4.5
ymin = -4.5
ymax = 4.5
zmin = -4.5
zmax = 4.5

fig = plt.figure()
ax = Axes3D(fig)
ax.plot(x1,y1,z1)
ax.plot(x2,y2,z2)
ax.plot(x3,y3,z3)
#plt.grid()
#plt.legend()
axes = plt.gca()
axes.set_xlim([xmin,xmax])
axes.set_ylim([ymin,ymax])
axes.set_zlim([zmin,zmax])
plt.show()

