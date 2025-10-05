import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('Meteorologic w co.csv')
df2 = pd.DataFrame(df['AveTemp'].to_numpy(), columns=['Average'])
print(df2)

plt.figure()
plt.plot(df['AveTemp'], color='red')
plt.plot(df['MaxTemp'], ls='dashed', color='blue')
plt.plot(df['MinTemp'], ls='dotted', color='green')
plt.legend(['AveTemp', 'MaxTemp','MinTemp'])
plt.savefig('ave_max_min')
print('"ave_max_min.png" has been created.')

plt.figure()
plt.hist(df['Precip'], color='black', bins=64)
plt.savefig('bar_chat')