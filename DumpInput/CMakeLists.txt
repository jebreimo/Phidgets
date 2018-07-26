cmake_minimum_required(VERSION 3.7)
project(DumpInput)

set(CMAKE_CXX_STANDARD 14)

if (APPLE)
    include_directories(/Users/janeb/Repositories/Phidgets/Phidgets)
    link_directories(/Users/janeb/Repositories/Phidgets/Phidgets/build)
endif(APPLE)

add_executable(DumpInput main.cpp PhidgetError.cpp PhidgetError.hpp VoltageRatioInput.cpp VoltageRatioInput.hpp)

target_link_libraries(DumpInput phidget22)
