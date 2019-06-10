include_directories(${Movaic_SOURCE_DIR}/movaic/include)

# Packages required to build project.
find_package( OpenCV REQUIRED )

add_executable(Movaic main.cpp)

target_link_libraries( Movaic ${OpenCV_LIBS} )

