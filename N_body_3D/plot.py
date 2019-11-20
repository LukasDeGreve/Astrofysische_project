import math as m
import numpy as np
import scipy.special
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
#from matplotlib import animation
from mpl_toolkits.mplot3d import Axes3D
import csv
#from moviepy.editor import VideoClip
from matplotlib.animation import FFMpegWriter


#Loading in data
p, x, y, z = np.genfromtxt("test.txt", unpack = True)
#print(p)
#print(x)
#print(y)
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

'''
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
#Animation
'''
def make_frame(t):
    """ returns an image of the frame at time t """
    # ... create the frame with any library
    return frame_for_time_t # (Height x Width x 3) Numpy array

animation = VideoClip(make_frame, duration=3) # 3-second clip

# For the export, many options/formats/optimizations are supported
animation.write_videofile("my_animation.mp4", fps=24) # export as video
animation.write_gif("my_animation.gif", fps=24) # export as GIF (slow)
t = 0.
h = 0.001
'''

metadata = dict(title='Movie Test', artist='Matplotlib',
                comment='Movie support!')
writer = FFMpegWriter(fps=60, metadata=metadata)

fig = plt.figure()
t, = plt.plot([], [], linewidth = 1)
l, = plt.plot([], [], 'k-o')
k, = plt.plot([], [], 'k-o')
s, = plt.plot([], [], 'k-o')

plt.xlim(-1.5, 1.5)
plt.ylim(-1.5, 1.5)

with writer.saving(fig, "writer_test.mp4", 100):
    for i in range(iterations):
        print(i)
        t.set_data(x3, y3)
        l.set_data(x1[i], y1[i])
        k.set_data(x2[i], y2[i])
        s.set_data(x3[i], y3[i])
        writer.grab_frame()
'''
for i in range(2):
    fig = plt.figure()
    ax = Axes3D(fig)
    ax.scatter(x1[i],y1[i],z1[i], label='Particle 1')
    ax.scatter(x2[i],y2[i],z2[i], label = 'Particle 2')
    ax.scatter(x3[i],y3[i],z3[i], label = 'Particle 3')
    plt.grid()
    plt.legend()
    make_frame(t)
    t += h
'''
