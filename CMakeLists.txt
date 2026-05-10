cmake_minimum_required(VERSION 3.10)
project(INS_GNSS_KF)

set(CMAKE_CXX_STANDARD 17)

find_package(Eigen3 REQUIRED)

include_directories(include)

add_executable(INS_GNSS_KF
    src/main.cpp
    src/bins_algoritm.cpp
    src/kalman_filter.cpp
    src/imu_parser.cpp
    src/gnss_parser.cpp
    src/logger.cpp
)

target_link_libraries(INS_GNSS_KF Eigen3::Eigen)