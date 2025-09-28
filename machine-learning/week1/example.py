import pandas as pd
import matplotlib.pyplot as plt

plot_df = pd.read_csv('plot_data.csv')
plt.figure(figsize=(8, 5))
plt.plot(plot_df['x'], plot_df['y'])
plt.xlabel('x')
plt.ylabel('y')
plt.tight_layout()
plt.show()
