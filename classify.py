import numpy as np
from sklearn.neighbors import KNeighborsClassifier

# 1. Complete Calibration Dataset [xtrue, ytrue, q67, q45]
# These are the exact mean log-ratios you provided for the 5x5 grid
data = np.array([
    [-20,-20,  0.49754,  -0.42559  ], [-20,-10,  0.0892097, -0.211436 ], 
    [-20,  0, -0.856061,  -0.024672 ], [-20, 10,  0.0444989,  0.195558  ], 
    [-20, 20,  0.462901,   0.408656  ], [-10,-20,  0.325084,  -0.0321269], 
    [-10,-10, -0.0684167,  0.148409  ], [-10,  0, -0.440405,  -0.00182652], 
    [-10, 10, -0.0708033, -0.164773  ], [-10, 20,  0.274611,   0.0082409 ], 
    [  0,-20,  0.0402463,  0.918928  ], [  0,-10,  0.0458008,  0.528741  ], 
    [  0,  0,  0.0849144, -0.00183544], [  0, 10,  0.0455734, -0.462418  ], 
    [  0, 20,  0.0212321, -0.979456  ], [ 10,-20, -0.237046,  -0.102362  ], 
    [ 10,-10,  0.0868794,  0.109224  ], [ 10,  0,  0.496961,  -0.0212576 ], 
    [ 10, 10,  0.156487,  -0.17857   ], [ 10, 20, -0.243097,  -0.0291829 ], 
    [ 20,-20, -0.420558,  -0.472336  ], [ 20,-10, -0.111018,  -0.253613  ], 
    [ 20,  0,  0.893376,  -0.0575828 ], [ 20, 10,  0.0093554,  0.155497  ], 
    [ 20, 20, -0.377241,   0.389246  ]
])

# Features: Raw charge ratios | Labels: 0 to 24 (Cell Index)
X_train = data[:, 2:4]
y_train = np.arange(25)
true_coords = data[:, 0:2]

# 2. Initialize and Train the k-NN Classifier
# k=1 means "Pick the single closest anchor point"
clf = KNeighborsClassifier(n_neighbors=1, metric='euclidean')
clf.fit(X_train, y_train)

# 3. Prediction Function
def classify_muon(q67_val, q45_val):
    """Returns the (x, y) of the most likely cell center"""
    test_point = np.array([[q67_val, q45_val]])
    class_idx = clf.predict(test_point)
    return true_coords[class_idx]

# Example Test: A hit near the center (0,0)
# Our center anchor was [0.0849, -0.0018]
predicted_xy = classify_muon(0.12, 0.5)
print(f"Muon hit identified at Cell Center: {predicted_xy} cm")
