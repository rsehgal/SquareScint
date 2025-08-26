import numpy as np
from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import LinearRegression
from sklearn.model_selection import cross_val_score
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D  # registers 3D

# === Replace these with your 25 calibration points ===
# Example placeholders (length 25)
delTx = np.array([ -1.5044, -2.8324, -0.0907, 1.4153, 2.2551, -1.1659, -2.2619, -0.2820, 0.9001, 1.6395,
                  -2.2089, -3.2038, -0.2451, 1.6500, 2.6565, -1.3898, -2.5648, -0.2089, 1.3294, 2.1691,
                  -0.6372, -1.1912, -0.0912, 0.6626, 1.1636])
delTy = np.array([2.3030, 1.0059, 2.7151, 1.7482, 0.6722, 2.4656, 0.9506, 3.0048, 1.6573, 0.9641,
                  1.0899, 0.7970, 1.1430, 0.9659, 0.6655, -0.1406, 0.6564, -0.6011, 0.0711, 0.4273,
                  -0.9922, 0.3235, -1.7094, -0.5876, 0.2567])
x_true = np.array([-20,-10,0,10,20,
-20,-10,0,10,20,
-20,-10,0,10,20,
-20,-10,0,10,20,
-20,-10,0,10,20])
y_true = np.array([20,20,20,20,20,
10,10,10,10,10,
0,0,0,0,0,
-10,-10,-10,-10,-10,
-20,-20,-20,-20,-20])

# Prepare feature matrix
X = np.vstack([delTx, delTy]).T

# Try degrees and compare with CV
for deg in (1, 2, 3):
    poly = PolynomialFeatures(degree=deg, include_bias=True)
    Xp = poly.fit_transform(X)
    model_x = LinearRegression().fit(Xp, x_true)
    model_y = LinearRegression().fit(Xp, y_true)
    # simple cv score on x and y
    scores_x = cross_val_score(LinearRegression(), Xp, x_true, cv=5, scoring='r2')
    scores_y = cross_val_score(LinearRegression(), Xp, y_true, cv=5, scoring='r2')
    print(f"deg={deg}: R2_x mean={scores_x.mean():.3f}, R2_y mean={scores_y.mean():.3f}")

# Pick a degree (say deg=2)
deg = 3
poly = PolynomialFeatures(degree=deg, include_bias=True)
Xp = poly.fit_transform(X)
model_x = LinearRegression().fit(Xp, x_true)
model_y = LinearRegression().fit(Xp, y_true)

# Function to predict position from new (delTx, delTy)
def predict_position(dtx, dty):
    v = np.array([[dtx, dty]])
    vp = poly.transform(v)
    x_pred = model_x.predict(vp)[0]
    y_pred = model_y.predict(vp)[0]
    return x_pred, y_pred

# Example predict
dtx_sample, dty_sample = 1.2, -0.5
print("predicted (x,y):", predict_position(dtx_sample, dty_sample))

# --- Optional: plot fitted surface for x (and similarly for y) ---
# create a grid
grid_n = 50
dtx_lin = np.linspace(delTx.min(), delTx.max(), grid_n)
dty_lin = np.linspace(delTy.min(), delTy.max(), grid_n)
D1, D2 = np.meshgrid(dtx_lin, dty_lin)
grid_pts = np.vstack([D1.ravel(), D2.ravel()]).T
grid_p = poly.transform(grid_pts)
X_surf = model_x.predict(grid_p).reshape(D1.shape)
Y_surf = model_y.predict(grid_p).reshape(D1.shape)

fig = plt.figure(figsize=(12,5))
ax = fig.add_subplot(121, projection='3d')
ax.plot_surface(D1, D2, X_surf, alpha=0.7)
ax.scatter(delTx, delTy, x_true, color='r')
ax.set_xlabel('delTx'); ax.set_ylabel('delTy'); ax.set_zlabel('x_true'); ax.set_title('Fitted surface -> x')

ax2 = fig.add_subplot(122, projection='3d')
ax2.plot_surface(D1, D2, Y_surf, alpha=0.7)
ax2.scatter(delTx, delTy, y_true, color='r')
ax2.set_xlabel('delTx'); ax2.set_ylabel('delTy'); ax2.set_zlabel('y_true'); ax2.set_title('Fitted surface -> y')

plt.tight_layout()
plt.show()

