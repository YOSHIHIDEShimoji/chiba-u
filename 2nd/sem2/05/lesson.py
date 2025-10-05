import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('Meteorologic w co.csv')
df2 = pd.DataFrame(df[['AveTemp', 'MaxTemp', 'MinTemp']].to_numpy(), columns=['AveTemp', 'MaxTemp', 'MinTemp'])

plt.figure()
plt.plot(df['AveTemp'], color='red')
plt.plot(df['MaxTemp'], ls='dashed', color='blue')
plt.plot(df['MinTemp'], ls='dotted', color='green')
plt.legend(['AveTemp', 'MaxTemp','MinTemp'])
plt.savefig('./_ignore/ave_max_min')

plt.figure()
plt.hist(df['Precip'], color='black', bins=64)
plt.savefig('./_ignore/bar_chat')

plt.figure()
plt.axes().set_aspect('equal')
plt.scatter(df2['AveTemp'], df2['MinTemp'], color='white', edgecolors='black')
plt.xlabel('Average temperature[°C]')
plt.ylabel('Minimum temperature[°C]')
plt.xlim([0, 40])
plt.ylim([-5, 35])
plt.savefig('./_ignore/scatter_diagram')

fig = plt.figure()
ax = fig.add_subplot(111)
bp = ax.boxplot(df['AveTemp'].to_numpy())
ax.set_xticklabels(['AveTemp'])
plt.savefig('./_ignore/box')



