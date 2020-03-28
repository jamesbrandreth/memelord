cmake_minimum_required(VERSION 3.15)
project(meme_lord)

set(CMAKE_CXX_STANDARD 17)

find_package( OpenCV REQUIRED )

add_executable(meme_lord main.cpp)
target_link_libraries( meme_lord ${OpenCV_LIBS} )
target_link_libraries( meme_lord tesseract )
