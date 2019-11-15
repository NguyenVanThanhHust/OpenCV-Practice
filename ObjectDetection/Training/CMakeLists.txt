cmake_minimum_required(VERSION 3.5.1)
project(FourGlobe2)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)
set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -D__LIBSWRESAMPLE__ -std=c++14 -pthread")

set(SOURCE_FILES
        train_HOG.h
	train_HOG.cpp
        main.cpp)

#cuda_add_executable(FourGlobe2 ${SOURCE_FILES})
find_package(OpenCV REQUIRED)
# find_package(FFmpeg REQUIRED)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${OPENCV_INCLUDE_DIRS})

# Build source files
add_executable(Training ${SOURCE_FILES})

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,/usr/lib/nvidia")

# Add libs
target_link_libraries(Training
        ${OpenCV_LIBS}
        ${CUDA_LIBRARIES}
)

