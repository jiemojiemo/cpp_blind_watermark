cmake_minimum_required(VERSION 3.15)

project(BlindWatermarkTest)

find_package(OpenCV CONFIG REQUIRED)
find_package(base64 CONFIG REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE opencv::opencv aklomp::base64)