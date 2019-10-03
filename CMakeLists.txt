cmake_minimum_required(VERSION 3.13)
project(moviac)

set(CMAKE_CXX_STANDARD 14)

set(include_dest "include")
set(tool_dest "bin")

# Packages required to build project.
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include(external/external.cmake)

add_executable(moviac main.cpp util.cpp util.h)

target_link_libraries(moviac ${OpenCV_LIBS})
target_link_libraries(moviac libHealthBar)
