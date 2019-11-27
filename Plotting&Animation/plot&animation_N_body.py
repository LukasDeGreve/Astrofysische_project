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
p, x, y, z = np.genfromtxt("/home/wout/Documents/Astro_sim/Project/Tests&Systems/Three_body_8/Three_body.txt", unpack = True)
N = 3
print_step = 10
timesteps = 7000
code_blocks = int(timesteps/print_step);
Data = np.zeros((code_blocks,3,N))

for i in range(code_blocks):
    for n in range(N):
        Data[i,0,n] = x[i+n]
        Data[i,1,n] = y[i+n]
        Data[i,2,n] = z[i+n]

#Plotting all the positions of all the particles

xmin = -1.5
xmax = 1.5
ymin = -1.5
ymax = 1.5
zmin = -1.5
zmax = 1.5


fig = plt.figure()
ax = Axes3D(fig)
for n in range(N):
    ax.scatter(Data[:,0,n], Data[:,1,n], Data[:,2,n],s=1)

plt.grid()
axes = plt.gca()
axes.set_xlim([xmin,xmax])
axes.set_ylim([ymin,ymax])
axes.set_zlim([zmin,zmax])
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')
plt.show()

#Animation of all the positions of all the particles
fig = plt.figure()
ax = Axes3D(fig)

skip = 1

#generating a color for every particle
colors = np.zeros((0))
for i in range(N):
    colors = np.append(colors,'#%06X' % randint(0, 0xFFFFFF))

def animate(i):
    print(i)
    ax.clear()
    axes = plt.gca()
    axes.set_xlim([xmin,xmax])
    axes.set_ylim([ymin,ymax])
    axes.set_zlim([zmin,zmax])

    for n in range(N):
        ax.scatter(Data[i*skip,0,n], Data[i*skip,1,n], Data[i*skip,2,n], c  = colors[n])

    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')
    ax.view_init(90,90)

anim = animation.FuncAnimation(fig, animate, frames = code_blocks, interval = 20, blit = False)
anim.save('N_body_3D.mp4', fps=2, extra_args=['-vcodec', 'libx264'])

