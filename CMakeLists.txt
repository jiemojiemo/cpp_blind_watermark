cmake_minimum_required(VERSION 3.15)

project(BlindWatermarkTest)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV CONFIG REQUIRED)
find_package(base64 CONFIG REQUIRED)

if (ENABLE_ASAN)
    message(STATUS "build with ASAN")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif ()

add_subdirectory(3rdparty)

add_library(my_lib INTERFACE)
target_include_directories(my_lib
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/wavelib/header)

target_link_libraries(my_lib INTERFACE wavelib opencv::opencv aklomp::base64)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE my_lib wavelib)

add_subdirectory(tests)