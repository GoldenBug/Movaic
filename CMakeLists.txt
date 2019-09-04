cmake_minimum_required(VERSION 3.14)
project(moviac)

set(CMAKE_CXX_STANDARD 14)

# Packages required to build project.
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(moviac main.cpp)

target_link_libraries(moviac ${OpenCV_LIBS})