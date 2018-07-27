cmake_minimum_required(VERSION 3.12)
project(VirtualmoniumAdapter)

if (APPLE)
    include_directories(/Users/janeb/Repositories/Phidgets/Phidgets)
    link_directories(/Users/janeb/Repositories/Phidgets/Phidgets/build)
endif(APPLE)

set(CMAKE_CXX_STANDARD 14)

add_executable(VirtualmoniumAdapter Source/main.cpp)
