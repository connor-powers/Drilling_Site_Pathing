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

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)

FetchContent_MakeAvailable(Eigen googletest)

add_executable(run_manual_map_demo src/manual_map_demo.cpp src/utils.cpp src/ManualRewardMap.cpp)
add_executable(run_random_map_demo src/random_map_demo.cpp src/utils.cpp src/RandomRewardMap.cpp)

add_executable(tests tests/tests.cpp src/utils.cpp src/ManualRewardMap.cpp)

target_link_libraries(run_manual_map_demo PRIVATE Eigen3::Eigen)
target_link_libraries(run_random_map_demo PRIVATE Eigen3::Eigen)

target_link_libraries(tests PRIVATE Eigen3::Eigen gtest_main)
