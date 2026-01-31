import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import ScalarFormatter
import matplotlib.ticker as ptick
from scipy import stats
from scipy import optimize
from sklearn import linear_model

df = pd.read_csv('Population.csv')
# print(df)

# linear_model
clf = linear_model.LinearRegression()
x = df['Year'].values
y = df['Marriages'].values
x2d = x.reshape(-1, 1)
clf.fit(x2d, y)

# polyfit
res = np.polyfit(x, y, 2)
yf = np.poly1d(res)(x)

# correlation coefficient
cc = np.corrcoef(y, yf)[0, 1]
print(cc)

# create_graph
plt.rcParams['font.size'] = 14
plt.rcParams['figure.dpi'] = 300
fig, ax = plt.subplots()
ax.scatter(df['Year'], df['Marriages'], color='white', edgecolors='black')
ax.plot(x, clf.predict(x2d), color='blue', label='Linear regression')
ax.plot(x, yf, color='red', label='Secondary regression')
ax.set_xlabel('Year')
ax.set_ylabel('Number of Marriages')
ax.yaxis.set_major_formatter(ptick.ScalarFormatter(useMathText=True))
ax.ticklabel_format(axis='y', style='sci', scilimits=(0, 0))
ax.legend(loc='best')

plt.tight_layout()
plt.savefig('./_ignore/pro.png')

