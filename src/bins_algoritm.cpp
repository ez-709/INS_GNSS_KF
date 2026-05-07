#include <cmath>
#include "bins_algoritm.hpp"

BINS_algoritm::BINS_algoritm(double yaw0) {
    C << cos(yaw0),  sin(yaw0),  0,
        -sin(yaw0),  cos(yaw0),  0,
         0,          0,          1;

    n_normal = Eigen::Vector3d::Zero();
}