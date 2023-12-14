cmake_minimum_required(VERSION 3.15)

project(BlindWatermarkTest)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV CONFIG REQUIRED)
find_package(base64 CONFIG REQUIRED)

add_library(my_lib INTERFACE)
target_include_directories(my_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(my_lib INTERFACE opencv::opencv aklomp::base64)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE my_lib)

add_subdirectory(tests)