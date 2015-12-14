cmake_minimum_required(VERSION 2.6)

# Build options are: None, Debug, Release, RelWithDefInfo, MinSizeRel
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_COMPILER g++)
project(TSPNUniformDisks)

# include C++11 standard
# optimize for speed
set(CMAKE_CXX_FLAGS "-std=c++0x -Wall -O3 -g")

# Set output dir for files
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs)

# main program
include_directories("./include")
add_executable(tspn_demo src/tspn_demo.cpp src/tour.cpp src/tsp.cpp src/tspnuniformdisks.cpp)
add_executable(tsp_demo src/tsp_demo.cpp src/tsp.cpp src/tour.cpp)
