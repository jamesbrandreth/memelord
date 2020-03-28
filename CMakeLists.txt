cmake_minimum_required(VERSION 3.15)
project(memelord)

set(CMAKE_CXX_STANDARD 17)

find_package( OpenCV REQUIRED )

add_executable(memelord main.cpp)
target_link_libraries( memelord ${OpenCV_LIBS} )
target_link_libraries( memelord tesseract )
