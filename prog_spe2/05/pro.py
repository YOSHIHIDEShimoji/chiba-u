import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('ecg4000.csv', skiprows=1, names=['Time', 'Voltage'])

plt.figure(figsize=(8, 3))
plt.plot(df['Time'], df['Voltage'], color='blue', lw=0.8)
plt.scatter(df['Time'][::50], df['Voltage'][::50], color='red', s=10, label='Sample Points (every 50)')
plt.title('ECG Signal')
plt.xlabel('Time [s]')
plt.ylabel('Voltage [mV]')
plt.grid(True)
plt.legend(loc='upper left')
plt.tight_layout()
plt.savefig('./ecg_waveform.png', dpi=300)

plt.figure(figsize=(6, 3))
plt.plot(df['Time'], df['Voltage'], color='blue', lw=0.8)
plt.scatter(df['Time'][::50], df['Voltage'][::50], color='red', s=10, label='Sample Points (every 50)')
plt.title('Zoomed ECG Signal')
plt.xlabel('Time [s]')
plt.ylabel('Voltage [mV]')
plt.xlim(1, 2)
plt.grid(True)
plt.legend(loc='upper right')
plt.tight_layout()
plt.savefig('./ecg_zoom.png', dpi=300)
