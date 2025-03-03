cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
include_directories(include)

project(drilling_site_pathing)
include(FetchContent)

FetchContent_Declare(
        Eigen
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
        GIT_TAG 3.4
)

FetchContent_MakeAvailable(Eigen)

add_executable(run src/core.cpp src/utils.cpp src/ManualRewardMap.cpp)

target_link_libraries(run PRIVATE Eigen3::Eigen)
