import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy import stats
from statsmodels.stats.multicomp import pairwise_tukeyhsd

df = pd.read_csv('AveTemp_2007-2017.csv')
# print(df.describe())

dmy = df.values
vals = dmy[:, 1:].astype(np.float64)
columns = df.columns
columns = columns[1:]

# 箱ひげ図
fig = plt.figure(figsize=(6, 4), dpi=300)
ax = fig.add_subplot(111)
bp = ax.boxplot(vals, widths=0.8)

for box in bp['boxes']:
    box.set(color='black', linewidth=1.0)
for box in bp['medians']:
    plt.setp(box, color='black', linewidth=1.0)
for box in bp['caps']:
    plt.setp(box, color='black', linewidth=1.0)
for box in bp['whiskers']:
    plt.setp(box, ls='solid', color='black', linewidth=1.0)

ax.set_xticklabels(columns)
plt.ylim([16, 34])
plt.savefig('./pro.png')

# 対応のないt検定
t, p = stats.ttest_ind(vals[:, 0], vals[:, 3], equal_var=False)
print('対応のないt検定', 'p =',  p)

# Bartlett検定 
print(stats.bartlett(vals[:, 0], vals[:, 1], vals[:, 2], vals[:, 3], vals[:, 4], 
                     vals[:, 5], vals[:, 6], vals[:, 7], vals[:, 8], vals[:, 9]))

# 一元配置分散分析
print(stats.f_oneway(vals[:, 0], vals[:, 1], vals[:, 2], vals[:, 3], vals[:, 4], 
                     vals[:, 5], vals[:, 6], vals[:, 7], vals[:, 8], vals[:, 9]))

# 対応のあるTukeyHSD検定
ind = np.repeat(columns, len(vals))
val_d = np.ravel(vals.T)
print(pairwise_tukeyhsd(val_d, ind))