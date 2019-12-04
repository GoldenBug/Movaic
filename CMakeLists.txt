cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 14)
include(ExternalProject)

project(moviac)

add_subdirectory(external/HealthBar)
add_subdirectory(external/boost-cmake)

# Packages required to build project.
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(moviac src/main.cpp src/util.cpp src/util.h src/commandLineParser.cpp src/commandLineParser.h)

target_link_libraries(moviac PUBLIC ${OpenCV_LIBS})
target_link_libraries(moviac PUBLIC healthbar)
target_link_libraries(moviac PUBLIC Boost::filesystem)
target_link_libraries(moviac PUBLIC Boost::boost)
target_link_libraries(moviac PUBLIC Boost::program_options)