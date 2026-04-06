import matplotlib.pyplot as plt

from utils import load_imu
from calculations import integrate_orientation
from plots import plot_timeseries, plot_3d_orientation

imu = load_imu()
roll, pitch, yaw = integrate_orientation(imu)

plot_timeseries(imu)
slider = plot_3d_orientation(imu, roll, pitch, yaw)

plt.show()