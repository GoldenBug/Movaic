cmake_minimum_required (VERSION 2.6)
project (Movaic)

# The version number.
set (Tutorial_VERSION_MAJOR 0)
set (Tutorial_VERSION_MINOR 1)

# Packages required to build project.
find_package( OpenCV REQUIRED )

add_executable(Movaic main.cpp)

target_link_libraries( Movaic ${OpenCV_LIBS} )
