import uproot
import numpy as np
import matplotlib.pyplot as plt

def baseline_subtract(pulse, n_baseline=16):
    """
    Subtracts baseline from a pulse.
    n_baseline: number of samples at start used to estimate baseline
    """
    baseline = np.median(pulse[:n_baseline])
    return pulse - baseline

def integrate_window(v, i0, i1, dt):
    """
    Integrate baseline-subtracted voltage samples v[i0:i1]
    using the trapezoidal rule. Returns area in V·s (use dt in seconds).
    """
    segment = v[i0:i1]
    return np.trapezoid(segment, dx=dt)

def find_window(v, frac=0.1, pre=10, post=80):
    """
    Find a window around the pulse peak.
    - frac: fraction of max for leading-edge threshold
    - pre/post: extra samples before/after threshold for safety
    Returns (i0, i1)
    """
    if np.max(np.abs(v)) == 0:
        return 0, len(v)
    # Detect polarity automatically (positive or negative pulse)
    if np.max(v) >= -np.min(v):
        peak_idx = np.argmax(v)
        thr = frac * v[peak_idx]
        # first sample going above thr
        crossings = np.where(v[:-1] < thr)[0]
        start = 0
        for k in crossings:
            if v[k+1] >= thr:
                start = k + 1
                break
    else:
        peak_idx = np.argmin(v)
        thr = frac * v[peak_idx]  # negative value
        crossings = np.where(v[:-1] > thr)[0]
        start = 0
        for k in crossings:
            if v[k+1] <= thr:
                start = k + 1
                break

    i0 = max(0, start - pre)
    i1 = min(len(v), peak_idx + post)
    return i0, i1

# open the ROOT file
#file = uproot.open("/home/rsehgal/SquareScint/CPP/build/MuonWaveTest.root")
file = uproot.open("../CPP/build/TestMuonWave.root")

# check the tree
tree = file["treeML"]

# list branches
print(tree.keys())

arr = tree["samples0"].array(library="np")
timing0 = tree["t0"].array(library="np")
q0 = tree["q0"].array(library="np")
print("Timing : ", timing0[0])
print("Actual Charge : ",q0[0])

print(type(arr))   # usually numpy.ndarray or awkward.Array
print(arr.shape)
print(arr[0]) 

pulse = arr[0]
x = np.arange(len(pulse)) * 2

# plot
#plt.figure(figsize=(8,4))
#plt.plot(x, pulse, marker="o", linestyle="-")
#plt.xlabel("Time (arb. units, step=2)")
#plt.ylabel("Amplitude (ADC counts)")
#plt.title("Pulse from TArrayS branch")
#plt.grid(True)


pulse_bs = baseline_subtract(pulse, n_baseline=16)
np.savetxt("pulse.txt",pulse_bs)

#plt.plot(x, pulse, label="Original")
#plt.plot(x, pulse_bs, label="Baseline subtracted")
#plt.grid(True)
#plt.legend()

plt.figure(2)
plt.plot(x, pulse, marker=".", linestyle="-")
plt.xlabel("Time (arb. units, step=2)")
plt.ylabel("Amplitude (ADC counts)")
plt.title("Pulse from TArrayS branch")
plt.grid(True)

gated_pulse = pulse_bs[38:60]
gated_x = x[38:60]
plt.figure(3)
plt.plot(x, pulse_bs, marker=".", linestyle="-")
plt.plot(gated_x, gated_pulse, marker=".",color='red', linestyle="-")
plt.xlabel("Time (arb. units, step=2)")
plt.ylabel("Amplitude (ADC counts)")
plt.title("Pulse from TArrayS branch")
plt.grid(True)

dt = 2e-9
i0, i1 = find_window(pulse_bs, frac=0.1, pre=10, post=120)
area_Vs = integrate_window(pulse_bs, i0, i1, dt)
Q_coulomb = area_Vs / 50.0
print("Calculated Charge : ",Q_coulomb)
print("Sum : ",np.sum(pulse_bs[:100]))


plt.show()

