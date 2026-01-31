import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv('LineProfile.csv')
orgV = df['Val1'].values
L = len(orgV)

Mx = np.max(orgV)
Mn = np.min(orgV)
orgV = (orgV - Mn) / (Mx - Mn)

HL = int(L / 2)
Idx = np.array([i for i in range(-HL, HL)])
Vf = np.fft.fft(orgV)
Vfs = np.fft.fftshift(Vf)
aVf = np.abs(Vfs)

# Process1
N = 80
pVfs = np.zeros((L), dtype=complex)
pVfs[(HL - N):(HL + N)] = Vfs[(HL - N):(HL + N)]
apVfs = np.abs(pVfs)

Vi = np.fft.ifft(np.fft.ifftshift(pVfs))
Vi_real = np.real(Vi)

plt.figure(figsize=(6, 3))
plt.plot(Vi_real, color='black', linestyle='solid', linewidth=0.8)
plt.xlabel('Sample')
plt.ylabel('Filtered signal')
plt.savefig('./Process1.png', dpi=300, bbox_inches='tight')
plt.close()

# Process2
SG = 50
GF = np.array([i for i in range(-HL, HL)])
GF = np.exp(-GF * GF / (2 * SG * SG))
MxGF = np.max(GF)
GF = GF / MxGF

GVfs = Vfs * GF
GVi = np.fft.ifft(np.fft.ifftshift(GVfs))
gvi = np.real(GVi)

plt.figure(figsize=(6, 3))
plt.plot(gvi, color='black', linestyle='solid', linewidth=0.8)
plt.xlabel('Sample index')
plt.ylabel('Amplitude')
plt.savefig('./Process2.png', dpi=300, bbox_inches='tight')
plt.close()

