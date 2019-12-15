import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("Benchmarks/Benchmark_NoTree.txt")
particles = []
time = []

plt.xscale("log")
plt.yscale("log")
plt.ylim(1, 25000)
plt.xlim(5, 5000)
for gegevens in data:
    time.append(gegevens[1])
    particles.append(gegevens[0])

def quad(x):
    return 0.0204394*x**2+0.50322*x-40.8278

x = np.logspace(0, 6, 500, dtype='float64')

plt.scatter(particles,time, color='k', marker='o', label="data points")
plt.plot(x, quad(x), '-k', label='Quadratic fit')
plt.ylabel('processing time (ms)')
plt.xlabel('# particles')
plt.title('integration time in function of the amount of particles')
plt.legend(loc='upper left')
plt.show()
