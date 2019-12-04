cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 14)
include(ExternalProject)

project(moviac)

add_subdirectory(external/HealthBar)

# Packages required to build project.
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(moviac src/main.cpp src/util.cpp src/util.h)

target_link_libraries(moviac ${OpenCV_LIBS})
target_link_libraries(moviac healthbar)
