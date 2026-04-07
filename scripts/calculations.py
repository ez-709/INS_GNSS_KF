import numpy as np

def matrix_of_direction_cos(psi=0, gamma=0, theta=0):
    Rx = np.array([
        [1,          0,           0         ],
        [0, np.cos(psi), -np.sin(psi)       ],
        [0, np.sin(psi),  np.cos(psi)       ]
    ])
    Ry = np.array([
        [ np.cos(gamma), 0, np.sin(gamma)],
        [ 0,             1, 0            ],
        [-np.sin(gamma), 0, np.cos(gamma)]
    ])
    Rz = np.array([
        [np.cos(theta), -np.sin(theta), 0],
        [np.sin(theta),  np.cos(theta), 0],
        [0,              0,             1]
    ])
    return Rz @ Ry @ Rx


def skew_symmetric(v):
    wx, wy, wz = v
    return np.array([
        [  0,  -wz,  wy],
        [ wz,    0, -wx],
        [-wy,   wx,   0]
    ], dtype=float)


def integrate_orientation(imu):
    t  = imu['timestamp']
    dt = np.diff(t, prepend=t[0])
    roll  = np.cumsum(imu['wx'] * dt)
    pitch = np.cumsum(imu['wy'] * dt)
    yaw   = np.cumsum(imu['wz'] * dt)
    return roll, pitch, yaw


def accel_norm(imu):
    return np.sqrt(imu['ax']**2 + imu['ay']**2 + imu['az']**2)

def mag_norm(imu):
    return np.sqrt(imu['mx']**2 + imu['my']**2 + imu['mz']**2)