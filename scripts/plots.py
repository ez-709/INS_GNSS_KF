import os
import folium
import matplotlib.pyplot as plt


def plot_imu(imu):
    fig, axes = plt.subplots(3, 1, figsize=(12, 8), sharex=True)
    fig.suptitle('IMU')

    axes[0].plot(imu['timestamp'], imu['wx'], label='wx')
    axes[0].plot(imu['timestamp'], imu['wy'], label='wy')
    axes[0].plot(imu['timestamp'], imu['wz'], label='wz')
    axes[0].set_ylabel('рад/с')
    axes[0].legend()

    axes[1].plot(imu['timestamp'], imu['nx'], label='nx')
    axes[1].plot(imu['timestamp'], imu['ny'], label='ny')
    axes[1].plot(imu['timestamp'], imu['nz'], label='nz')
    axes[1].set_ylabel('м/с²')
    axes[1].legend()

    axes[2].plot(imu['timestamp'], imu['mx'], label='mx')
    axes[2].plot(imu['timestamp'], imu['my'], label='my')
    axes[2].plot(imu['timestamp'], imu['mz'], label='mz')
    axes[2].set_ylabel('мТл')
    axes[2].set_xlabel('с')
    axes[2].legend()

    plt.tight_layout()
    plt.show()


def plot_kf(kf):
    states = ['x1', 'x2', 'x3', 'x4', 'x5', 'x6', 'x7']
    sigs   = ['s1', 's2', 's3', 's4', 's5', 's6', 's7']
    labels = ['E, м', 'N, м', 'vE, м/с', 'vN, м/с', 'α, рад', 'β, рад', 'γ, рад']

    fig, axes = plt.subplots(7, 1, figsize=(12, 18), sharex=True)
    fig.suptitle('КФ')

    for ax, x, s, label in zip(axes, states, sigs, labels):
        ax.plot(kf['timestamp'], kf[x], linewidth=0.8)
        ax.fill_between(kf['timestamp'],
                        kf[x] - 3 * kf[s],
                        kf[x] + 3 * kf[s],
                        alpha=0.3)
        ax.set_ylabel(label, fontsize=8)
        ax.tick_params(labelsize=7)

    axes[-1].set_xlabel('с')
    plt.tight_layout()
    plt.show()


def plot_map(bins, gnss):
    bins = bins.dropna()
    gnss = gnss.dropna()

    bins_track = list(zip(bins['lat_deg'], bins['lon_deg']))
    gnss_track = list(zip(gnss['lat_deg'], gnss['lon_deg']))

    lat0 = gnss['lat_deg'].mean()
    lon0 = gnss['lon_deg'].mean()

    m = folium.Map(location=[lat0, lon0], zoom_start=10, tiles="CartoDB positron")

    folium.PolyLine(bins_track[::10], color="blue", weight=2,
                    opacity=0.8, tooltip="БИНС").add_to(m)
    folium.PolyLine(gnss_track[::10], color="red",  weight=2,
                    opacity=0.8, tooltip="ГНСС").add_to(m)

    folium.Marker(bins_track[0],  popup="БИНС Старт", icon=folium.Icon(color="blue")).add_to(m)
    folium.Marker(bins_track[-1], popup="БИНС Финиш", icon=folium.Icon(color="blue", icon="stop")).add_to(m)
    folium.Marker(gnss_track[0],  popup="ГНСС Старт", icon=folium.Icon(color="red")).add_to(m)
    folium.Marker(gnss_track[-1], popup="ГНСС Финиш", icon=folium.Icon(color="red",  icon="stop")).add_to(m)

    path = os.path.abspath("map.html")
    m.save(path)
    return path


def plot_accel_calibration(t, acc, acc_cal):
    fig, axes = plt.subplots(1, 2, figsize=(12, 5), subplot_kw={'projection': '3d'})
    axes[0].scatter(*acc.T,     s=1, alpha=0.3)
    axes[0].set_title('raw')
    axes[1].scatter(*acc_cal.T, s=1, alpha=0.3, c='tomato')
    axes[1].set_title('cal')
    plt.tight_layout()
    plt.show()

    fig, ax = plt.subplots(figsize=(14, 4))
    for col, label in zip(range(3), 'xyz'):
        ax.plot(t, acc_cal[:, col], lw=0.5, label=label)
    ax.set_ylabel('м/с²')
    ax.set_xlabel('с')
    ax.legend()
    ax.grid(True)
    plt.tight_layout()
    plt.show()


def plot_calibration(t, gyr, acc):
    fig, axes = plt.subplots(2, 1, figsize=(12, 5), sharex=True)
    fig.suptitle('Калибровка')

    for i, l in enumerate('xyz'):
        axes[0].plot(t, gyr[:, i], label=f'w{l}')
        axes[1].plot(t, acc[:, i], label=f'n{l}')

    axes[0].set_ylabel('рад/с')
    axes[1].set_ylabel('м/с²')
    axes[1].set_xlabel('с')

    for ax in axes:
        ax.legend()
        ax.grid(True)

    plt.tight_layout()
    plt.show()