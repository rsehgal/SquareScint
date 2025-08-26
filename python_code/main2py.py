from refactored_m4 import * #ProcessData, get_channels_from_config
from mainpy import run_analysis
from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import LinearRegression
from mpl_toolkits.mplot3d import Axes3D


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
#plt.show()

#-----------------------------------------------------------------------------------------
'''
# 3. Create a meshgrid over the data range
x_min, x_max = np.min(x_means) - 1, np.max(x_means) + 1
y_min, y_max = np.min(y_means) - 1, np.max(y_means) + 1

x_range = np.linspace(x_min, x_max, 100)
y_range = np.linspace(y_min, y_max, 100)
x_grid, y_grid = np.meshgrid(x_range, y_range)

# -----------------------------
# 4. Create polynomial features for the grid
X = np.vstack((x_grid.ravel(), y_grid.ravel())).T
poly = PolynomialFeatures(degree=2)
X_poly = poly.fit_transform(X)

# -----------------------------
# 5. Create random polynomial coefficients (no fitting)
np.random.seed(42)
weights = np.random.randn(X_poly.shape[1])  # Use same number of terms as features

z = X_poly @ weights
z_grid = z.reshape(x_grid.shape)

# -----------------------------
# 6. Plot the surface + the original scatter points
fig = plt.figure(figsize=(10, 7))
ax = fig.add_subplot(111, projection='3d')

# Surface
ax.plot_surface(x_grid, y_grid, z_grid, cmap='coolwarm', alpha=0.9)

# Original data points (z = 0 for display only)
ax.scatter(x_means, y_means, np.zeros_like(x_means), color='black', s=50, label='Data points')

# Labels & limits
ax.set_xlabel("x_mean (TimeDiff_1_3)")
ax.set_ylabel("y_mean (TimeDiff_0_2)")
ax.set_zlabel("z (random polynomial)")
ax.set_xlim([x_min, x_max])
ax.set_ylim([y_min, y_max])
ax.set_zlim([np.min(z_grid), np.max(z_grid)])
plt.title("2D Polynomial Surface over run_analysis Data")
plt.legend()
plt.show()


# 1. Create a 2D meshgrid
x_range = np.linspace(-5, 5, 100)
y_range = np.linspace(-5, 5, 100)
x_grid, y_grid = np.meshgrid(x_range, y_range)

# 2. Stack grid into shape (N, 2)
X = np.vstack((x_grid.ravel(), y_grid.ravel())).T

# 3. Generate polynomial features (e.g., degree 2)
poly = PolynomialFeatures(degree=2)
X_poly = poly.fit_transform(X)

# 4. Make up arbitrary weights for the features (just for plotting a surface)
# No need to know what each coefficient does
np.random.seed(0)
weights = np.random.randn(X_poly.shape[1])  # Random coefficients

# 5. Compute z from the random polynomial
z = X_poly @ weights
z_grid = z.reshape(x_grid.shape)

# 6. Plot the surface
fig = plt.figure(figsize=(10, 7))
ax = fig.add_subplot(111, projection='3d')
ax.plot_surface(x_grid, y_grid, z_grid, cmap='coolwarm', alpha=0.9)

ax.set_xlabel("x")
ax.set_ylabel("y")
ax.set_zlabel("z (random polynomial)")
#ax.set_zlim([-20, 20])
plt.title("Random 2D Polynomial Surface (no fitting, no manual coeffs)")
plt.show()



z_means = np.array([-20,-10,0,10,20])

x_means = x_means[:5]
y_means = y_means[:5]

z_means = np.sin(x_means) + np.cos(y_means)
#z_means = np.array([-20,-10,0,-10,20])
# ====== (3) Fit a 2D Polynomial Surface ======
# Prepare input for regression
X = np.vstack((x_means, y_means)).T
poly = PolynomialFeatures(degree=2)
X_poly = poly.fit_transform(X)

# Fit polynomial regression model
model = LinearRegression().fit(X_poly, z_means)

# Create a meshgrid for plotting the surface
x_range = np.linspace(-5, 5, 100)
y_range = np.linspace(-5, 5, 100)
x_grid, y_grid = np.meshgrid(x_range, y_range)

# Predict z-values on the grid
X_test = np.vstack((x_grid.ravel(), y_grid.ravel())).T
X_test_poly = poly.transform(X_test)
z_pred = model.predict(X_test_poly).reshape(x_grid.shape)

# ====== (4) Plot the 3D Surface ======
fig = plt.figure(figsize=(10, 7))
ax = fig.add_subplot(111, projection='3d')
ax.plot_surface(x_grid, y_grid, z_pred, cmap='viridis', alpha=0.9)
ax.set_xlabel("x_mean (TimeDiff_1_3)")
ax.set_ylabel("y_mean (TimeDiff_0_2)")
ax.set_zlabel("Estimated z (Surface)")
 #ax.set_xlim([-5, 5])
#ax.set_ylim([-5, 5])
#ax.set_zlim([-3, 3])
print("Estimated z (z_pred):")
print(z_pred)
plt.title("Fitted 2D Polynomial Surface")
plt.show()
'''

