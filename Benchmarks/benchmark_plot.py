import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("Benchmark_Delta0.8.txt")
particles = []
time = []

plt.xscale("log")
plt.yscale("log")
plt.ylim(50, 500000)
plt.xlim(5, 30000)
for gegevens in data:
    time.append(gegevens[1])
    particles.append(gegevens[0])

def log(x):
    return 4.44*x*np.log(0.015*x)

def quad(x):
    return 0.0087*x**2+7.76*x+21.251

x = np.logspace(0, 7, 1000, dtype='float64')

plt.scatter(particles,time, color='k', marker='o', label="data points")
plt.plot(x, log(x), ':k', label='Nlog(N) fit')
plt.plot(x, quad(x), '-k', label='Quadratic fit')
plt.ylabel('processing time (ms)')
plt.xlabel('# particles')
plt.title('integration time in function of the amount of particles')
plt.legend(loc='upper left')
plt.show()
