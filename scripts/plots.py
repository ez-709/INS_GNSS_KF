import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.widgets import Slider

from calculations import matrix_of_direction_cos, mag_norm

def plot_timeseries(imu):
    t = imu['timestamp']

    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(14, 10), sharex=True)
    fig.suptitle("IMU", fontsize=13)

    ax1.plot(t, imu['wx'], label='wx')
    ax1.plot(t, imu['wy'], label='wy')
    ax1.plot(t, imu['wz'], label='wz')
    ax1.set_ylabel('rad/s')
    ax1.set_title('Gyroscope')
    ax1.legend(); ax1.grid(True)

    ax2.plot(t, imu['ax'], label='ax')
    ax2.plot(t, imu['ay'], label='ay')
    ax2.plot(t, imu['az'], label='az')
    ax2.set_ylabel('m/s²')
    ax2.set_title('Accelerometer')
    ax2.legend(); ax2.grid(True)

    ax3.plot(t, imu['mx'], label='mx')
    ax3.plot(t, imu['my'], label='my')
    ax3.plot(t, imu['mz'], label='mz')
    ax3.set_ylabel('Gs')
    ax3.set_title('Magnetometer')
    ax3.set_xlabel('Time, s')
    ax3.legend(); ax3.grid(True)

    ax4.plot(t, mag_norm(imu))
    ax4.set_ylabel('Gs')
    ax4.set_title('Mag norm')
    ax4.set_xlabel('Time, s')
    ax4.legend(); ax4.grid(True)

    fig.tight_layout()

def plot_3d_orientation(imu, roll, pitch, yaw):
    t = imu['timestamp']
    n = len(t)

    fig = plt.figure(figsize=(10, 8))
    ax3d = fig.add_subplot(111, projection='3d')
    fig.subplots_adjust(bottom=0.15)

    origin = np.zeros(3)

    def draw_frame(idx):
        ax3d.cla()
        R = matrix_of_direction_cos(psi=roll[idx], gamma=pitch[idx], theta=yaw[idx])
        ax3d.quiver(*origin, *R[:, 0], color='r', length=1, normalize=True, linewidth=2.5, label='X')
        ax3d.quiver(*origin, *R[:, 1], color='g', length=1, normalize=True, linewidth=2.5, label='Y')
        ax3d.quiver(*origin, *R[:, 2], color='b', length=1, normalize=True, linewidth=2.5, label='Z')
        ax3d.set_xlim(-1.2, 1.2)
        ax3d.set_ylim(-1.2, 1.2)
        ax3d.set_zlim(-1.2, 1.2)
        ax3d.set_xlabel('X'); ax3d.set_ylabel('Y'); ax3d.set_zlabel('Z')
        ax3d.set_title(
            f't = {t[idx]:.3f} s    '
            f'roll={np.degrees(roll[idx]):.1f}°  '
            f'pitch={np.degrees(pitch[idx]):.1f}°  '
            f'yaw={np.degrees(yaw[idx]):.1f}°'
        )
        ax3d.legend(loc='upper left')
        fig.canvas.draw_idle()

    slider_ax = fig.add_axes([0.15, 0.04, 0.70, 0.03])
    slider = Slider(slider_ax, 'Time', 0, n - 1, valinit=0, valstep=1)
    slider.on_changed(lambda val: draw_frame(int(val)))

    draw_frame(0)
    return slider
