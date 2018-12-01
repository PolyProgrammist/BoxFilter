cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

project( BoxFilter )
find_package( OpenCV REQUIRED )
add_executable( BoxFilter main.cpp box_filter.cpp box_filter.h)
target_link_libraries( BoxFilter ${OpenCV_LIBS} )