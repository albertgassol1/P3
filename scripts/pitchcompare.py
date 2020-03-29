import matplotlib.pyplot as plt
import numpy as np

file = open("../pav_2341.f0", 'r')

valuesf0 = []
interval = 0.015
time = 0.15

for line in file.readlines():
    valuesf0.append(float(line))
    time+=interval
file.close()

file = open("../pav_2341.f0ref", 'r')

valuesf0ref = []
time = 0.015
for line in file.readlines():
    valuesf0ref.append(float(line))
    time+=interval

file.close()

fig, axs = plt.subplots(2)
fig.suptitle('Pitch calculado y pitch de referencia')
axs[0].plot(valuesf0, 'ro', markersize = 2)
axs[1].plot(valuesf0ref, 'bo', markersize = 2)
plt.show()