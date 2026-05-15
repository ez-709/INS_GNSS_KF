import numpy as np
from utils import read_csv
from plots import plot_calibration

dt = 0.01

t, gyr, acc, mag = read_csv('../data/imu_static.csv')

gyro_std  = np.std(gyr, axis=0)
accel_std = np.std(acc, axis=0)
gyro_q    = gyro_std**2  / dt
accel_q   = accel_std**2 / dt

print("gyro  std [rad/s]:", gyro_std.round(5))
print("accel std [m/s2]: ", accel_std.round(5))
print("gyro  [deg/hr]:   ", (gyro_std * 180/np.pi * 3600).round(1))

print(f"""
kalman_filter.cpp:
    Qw(0,0) = {gyro_q[0]:.6e};
    Qw(1,1) = {gyro_q[1]:.6e};
    Qw(2,2) = {gyro_q[2]:.6e};
    Qw(3,3) = {accel_q[0]:.6e};
    Qw(4,4) = {accel_q[1]:.6e};
    Qw(5,5) = {accel_q[2]:.6e};
""")

plot_calibration(t, gyr, acc)