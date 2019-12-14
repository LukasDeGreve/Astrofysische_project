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
p, x, y, z = np.genfromtxt("/home/wout/Documents/Astro_sim/Project/Fatsoenlijke_data/3_body/Positions_3_body_h=0.001000e=0.000000.txt", unpack = True)
N = 3                                  #Amount of particles
h = 0.001
e = 0
print_step = 10                         #The write_step variable from N_body_3D.cpp
timesteps = 25000                        #time_steps from N_body_3D.cpp
code_blocks = int(timesteps/print_step) #amount of txt blocks/amount of time steps

#Rearranging data for plotting
Data = np.zeros((code_blocks,3,N))
for i in range(code_blocks):
    for n in range(N):
        Data[i,0,n] = x[i*N+n]
        Data[i,1,n] = y[i*N+n]
        Data[i,2,n] = z[i*N+n]

#Plotting all the positions of all the particles
#Axes limits
xmin = -1.5
xmax = 1.5
ymin = -1.5
ymax = 1.5
zmin = -1.5
zmax = 1.5
'''
#Creating figure for full path of all particles
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
'''
#Animation of all the positions of all the particles
fig = plt.figure()
ax = Axes3D(fig)

#skip data points
skip = 1
#trail length for particles
trail = 15
size_particle = 0.5
#generating a color for every particle
colors = np.zeros((0))
for i in range(N):
    colors = np.append(colors,'#%06X' % randint(0, 0xFFFFFF))

#function for creating each frame of the animation (acts as loop)
def animate(i):
    print("Frame {}/{}".format(i,int(code_blocks/skip)-1))

    #Plotting full trajectory of first partcle (is useful for three body in 8 shape example)
    #ax.scatter(Data[:,0,0], Data[:,1,0], Data[:,2,0], c  = 'black',s=0.1)

    #Plotting position of every particle
    if i < trail:
        axes = plt.gca()
        axes.set_xlim([xmin,xmax])
        axes.set_ylim([ymin,ymax])
        axes.set_zlim([zmin,zmax])
        for n in range(N):
            ax.scatter(Data[i*skip,0,n], Data[i*skip,1,n], Data[i*skip,2,n], c  = colors[n], s = size_particle)
    else:
        ax.clear()
        axes = plt.gca()
        axes.set_xlim([xmin,xmax])
        axes.set_ylim([ymin,ymax])
        axes.set_zlim([zmin,zmax])
        for n in range(N):
            ax.scatter(Data[(i-trail)*skip:i*skip,0,n], Data[(i-trail)*skip:i*skip,1,n], Data[(i-trail)*skip:i*skip,2,n], c  = colors[n], s = size_particle)

    ax.set_xlabel('x')
    ax.set_ylabel('y')
    #ax.set_zlabel('z')
   
    ax.text2D(0.5,0.85,'Three body problem',fontsize=14,horizontalalignment='center',verticalalignment='center',transform=ax.transAxes)
    ax.text2D(0.1,0.9,'t={:.3f} s'.format(round(i*h*skip*print_step,4)),bbox=dict(facecolor='r',alpha=0.5),horizontalalignment='center',verticalalignment='center',transform=ax.transAxes)

    # Hide grid lines
    ax.grid(False)

    # Hide axes ticks
    #ax.set_xticks([])
    #ax.set_yticks([])
    ax.set_zticks([])

    # Get rid of colored axes planes
    # First remove fill
    ax.xaxis.pane.fill = False
    ax.yaxis.pane.fill = False
    ax.zaxis.pane.fill = False

    # Now set color to white (or whatever is "invisible")
    ax.xaxis.pane.set_edgecolor('w')
    ax.yaxis.pane.set_edgecolor('w')
    ax.zaxis.pane.set_edgecolor('w')
    ax.w_zaxis.line.set_color((1.0,1.0,1.0,0.0))

    #Change viewing angle
    #xy plane: 90,90
    ax.view_init(90,90)

#Make and save animation
anim = animation.FuncAnimation(fig, animate, frames = code_blocks, interval = 20, blit = False)
anim.save('3_body_h={}_e={}.mp4'.format(h,e), fps=30, extra_args=['-vcodec', 'libx264'])

