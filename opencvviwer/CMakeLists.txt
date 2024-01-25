cmake_minimum_required(VERSION 3.6)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

project(OpenCVViewer
	DESCRIPTION "OPENCV viewer"
	LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 17)

set (CMAKE_CXX_FLAGS "-Wall -Winvalid-pch -Wnon-virtual-dtor -Wextra -Wno-unused-parameter")

find_package(PkgConfig)

find_package(OpenCV REQUIRED)

pkg_check_modules(LIBCAMERA REQUIRED IMPORTED_TARGET libcamera)
message(STATUS "libcamera library found:")
message(STATUS "    version: ${LIBCAMERA_VERSION}")
message(STATUS "    libraries: ${LIBCAMERA_LINK_LIBRARIES}")
message(STATUS "    include path: ${LIBCAMERA_INCLUDE_DIRS}")

message(STATUS "opencv:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

include_directories(${CMAKE_SOURCE_DIR} ${LIBCAMERA_INCLUDE_DIRS} ${OPENCV_INCLUDE_DIRS})
include(GNUInstallDirs)

add_executable(opencv-viewer
  opencvviewer.cpp
)

target_link_libraries(opencv-viewer PkgConfig::LIBCAMERA)
target_link_libraries(opencv-viewer ${OpenCV_LIBS})
target_link_libraries(opencv-viewer cam2opencv)
