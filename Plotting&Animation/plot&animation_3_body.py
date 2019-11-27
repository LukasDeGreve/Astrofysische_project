import math as m
import numpy as np
import scipy.special
import matplotlib
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import csv
from matplotlib import animation


#Loading in data
p, x, y, z = np.genfromtxt("/home/wout/Documents/Astro_sim/Project/N_body_3D_versions/Positions.txt", unpack = True)
N = 3
print_step = 10
iterations = int(7000/print_step);
x1 = np.zeros((iterations))
y1 = np.zeros((iterations))
x2 = np.zeros((iterations))
y2 = np.zeros((iterations))
x3 = np.zeros((iterations))
y3 = np.zeros((iterations))
z1 = np.zeros((iterations))
z2 = np.zeros((iterations))
z3 = np.zeros((iterations))

i = 0
j = 0
k = 1
l = 2

while i < iterations:
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

#Plotting all the positions of all the particles
'''
xmin = -4.5
xmax = 4.5
ymin = -4.5
ymax = 4.5
zmin = -4.5
zmax = 4.5


fig = plt.figure()
ax = Axes3D(fig)
ax.plot(x1,y1,z1, label='Deeltje 1')
ax.plot(x2,y2,z2, label = 'Deeltje 2')
ax.plot(x3,y3,z3, label = 'Deeltje 3')
plt.grid()
plt.legend()
axes = plt.gca()
#axes.set_xlim([xmin,xmax])
#axes.set_ylim([ymin,ymax])
#axes.set_zlim([zmin,zmax])
#plt.plot(x1,y1)
#plt.plot(x2,y2)
#plt.plot(x3,y3)
plt.show()
'''


#Animation of all the positions of all the particles
fig = plt.figure()
ax = Axes3D(fig)

length = 1.5

xmin = -length
xmax = length
ymin = -length
ymax = length
zmin = -length
zmax = length

skip = 10

def animate(i):
    print(i)
    ax.clear()
    axes = plt.gca()
    axes.set_xlim([xmin,xmax])
    axes.set_ylim([ymin,ymax])
    axes.set_zlim([zmin,zmax])
    ax.scatter(x1[i*skip],y1[i*skip],z1[i*skip], c  = 'r')
    ax.scatter(x2[i*skip],y2[i*skip],z2[i*skip], c  = 'g')
    ax.scatter(x3[i*skip],y3[i*skip],z3[i*skip], c  = 'b')
    ax.plot(x1,y1,z1, c = 'black')
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')
    ax.view_init(90,90)

anim = animation.FuncAnimation(fig, animate, frames = iterations, interval = 20, blit = False)
anim.save('test.mp4', fps=30, extra_args=['-vcodec', 'libx264'])
    
