cmake_minimum_required(VERSION 2.6)
project(main)

include_directories(/usr/local/include)
include_directories(/usr/local/include/opencv)
include_directories(/usr/local/include/opencv2)

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp image.cpp vector3.cpp api.cpp)
add_executable(main ${SOURCE_FILES})

find_package(OpenCV REQUIRED)
target_link_libraries(main ${OpenCV_LIBS})
target_link_libraries(main -lgmp -lCGAL -lCGAL_Core)
