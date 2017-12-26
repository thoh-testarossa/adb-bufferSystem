cmake_minimum_required(VERSION 3.9)
project(adb-BufferSystem)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/IOCount.cpp src/bFrame.h src/DSMgr.h src/BMgr.h src/config.h src/BCB.h src/DSMgr.cpp src/BMgr.cpp)
add_executable(adb-BufferSystem-IOCount ${SOURCE_FILES})

set(SOURCE_FILES src/dataGen.cpp src/config.h)
add_executable(dataGen ${SOURCE_FILES})