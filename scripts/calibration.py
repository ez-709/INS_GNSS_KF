import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import minimize

G = 9.80665

def read_csv(fname):
    raw = np.genfromtxt(fname, delimiter=',', skip_header=1, invalid_raise=False)
    raw = raw[~np.isnan(raw).any(axis=1)]
    t   = raw[:, 0] - raw[0, 0]
    gyr = raw[:, 1:4]
    acc = raw[:, 4:7]
    mag = raw[:, 7:10]
    return t, gyr, acc, mag

def calibrate(data):
    def loss(params):
        Mx, My, Mz, dx, dy, dz, Cxy, Cxz, Cyx, Cyz, Czx, Czy = params
        M = np.array([[Mx,  Cxy, Cxz],
                      [Cyx, My,  Cyz],
                      [Czx, Czy, Mz]])
        d = np.array([dx, dy, dz])
        cal = (M @ (data - d).T).T
        return np.sum((np.sum(cal**2, axis=1) - G**2)**2)

    x0 = np.array([1., 1., 1., 0., 0., 0., 0., 0., 0., 0., 0., 0.])
    res = minimize(loss, x0)
    Mx, My, Mz, dx, dy, dz, Cxy, Cxz, Cyx, Cyz, Czx, Czy = res.x
    M = np.array([[Mx,  Cxy, Cxz],
                  [Cyx, My,  Cyz],
                  [Czx, Czy, Mz]])
    d = np.array([dx, dy, dz])
    return M, d

t, gyr, acc, mag = read_csv('data/imu_for_calib.csv')

M_acc, d_acc = calibrate(acc)
acc_cal = (M_acc @ (acc - d_acc).T).T

print("M =", M_acc)
print("d =", d_acc)

fig, axes = plt.subplots(1, 2, figsize=(12, 5), subplot_kw={'projection': '3d'})
axes[0].scatter(*acc.T,     s=1, alpha=0.3);              axes[0].set_title('accel raw')
axes[1].scatter(*acc_cal.T, s=1, alpha=0.3, c='tomato'); axes[1].set_title('accel calibrated')
plt.tight_layout(); plt.show()

fig, ax = plt.subplots(figsize=(14, 4))
for col, label in zip(range(3), 'xyz'):
    ax.plot(t, acc_cal[:, col], lw=0.5, label=label)
ax.set_title('accel calibrated')
ax.legend(); ax.grid(True)
plt.tight_layout(); plt.show()