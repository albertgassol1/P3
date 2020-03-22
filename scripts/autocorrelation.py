import matplotlib.pyplot as plt
import numpy as np
import wave
import sys

spf = wave.open("sonoro.wav", "r")
fig, axs = plt.subplots(2)
fig.suptitle('Senyal normal y autocorrelacion')
# Extract Raw Audio from Wav File
signal = spf.readframes(-1)
signal = np.fromstring(signal, "Int16")
signal = signal[1000:6000]
# If Stereo
if spf.getnchannels() == 2:
    print("Just mono files")
    sys.exit(0)

axs[0].plot(signal, 'tab:red')

r = []
for k in range(0, len(signal)):
    r.append(0)
    for n in range(0, len(signal) - k - 1):
        r[k] = r[k] + np.float(signal[n])*np.float(signal[n + k])
axs[1].plot(r)

plt.show()