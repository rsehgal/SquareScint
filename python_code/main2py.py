# SIMPLE PLOTTING OF PRECDICTED POSTION SURFACE 1D
# WHICH NOT GOT REALLY USED OR APPLIED 

from refactored_m4 import * #ProcessData, get_channels_from_config
from mainpy import run_analysis
import matplotlib.pyplot as plt



x_means, y_means, *_ = run_analysis()

# Now you can use them for scatter plots, predictions, etc.

plt.figure()
plt.scatter( x_means, y_means)
#plt.xlabel("x_mean (TimeDiff_1_3)")
plt.ylabel("y_mean (TimeDiff_0_2)")
plt.xlim(-5,5)
plt.ylim(-5,5)
plt.title("Scatter of Time Differences")
plt.grid(True)
plt.show()




