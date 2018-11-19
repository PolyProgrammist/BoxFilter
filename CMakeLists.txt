cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

project( MedianFilter )
find_package( OpenCV REQUIRED )
add_executable( MedianFilter main.cpp median_filter.cpp median_filter.h)
target_link_libraries( MedianFilter ${OpenCV_LIBS} )