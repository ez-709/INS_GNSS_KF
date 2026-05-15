from utils import read_csv
from calculations import calibrate
from plots import plot_accel_calibration

t, gyr, acc, mag = read_csv('data/imu_for_calib.csv')

M_acc, d_acc = calibrate(acc)
acc_cal = (M_acc @ (acc - d_acc).T).T

print("M =", M_acc)
print("d =", d_acc)

plot_accel_calibration(t, acc, acc_cal)