from refactored_m4 import * #ProcessData, get_channels_from_config
from mainpy import run_analysis



x_means, y_means = run_analysis()

# Now you can use them for scatter plots, predictions, etc.
import matplotlib.pyplot as plt
plt.figure()
plt.scatter(x_means, y_means)
plt.xlabel("x_mean (TimeDiff_1_3)")
plt.ylabel("y_mean (TimeDiff_0_2)")
plt.xlim(-5,5)
plt.ylim(-5,5)
plt.title("Scatter of Time Differences")
plt.grid(True)
plt.show()




