import numpy as np

stddev = 0

data = np.loadtxt("BenchmarkData.txt")

totaal = np.sum(data)
avg = totaal / 77

for getal in data:
    stddev += (getal - avg)**2

stddev /= 76
stddev = np.sqrt(stddev)

print(avg)
print(stddev)