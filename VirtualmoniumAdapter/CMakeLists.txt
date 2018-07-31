cmake_minimum_required(VERSION 3.7)
project(VirtualmoniumAdapter LANGUAGES C CXX)

if (APPLE)
    include_directories(/Users/janeb/Repositories/Phidgets/Phidgets)
    link_directories(/Users/janeb/Repositories/Phidgets/Phidgets/build)
endif(APPLE)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME}
    Source/main.cpp
    Source/Configuration.cpp
    Source/Configuration.hpp
    Source/PhidgetError.cpp
    Source/PhidgetError.hpp
    Source/VoltageRatioInput.cpp
    Source/VoltageRatioInput.hpp
    Source/tinyosc.c
    Source/tinyosc.h
    Source/FaderMessageQueue.cpp Source/FaderMessageQueue.hpp Source/UdpConnection.cpp Source/UdpConnection.hpp Source/Utilities.cpp Source/Utilities.hpp)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/External/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/External/include)

target_link_libraries(${PROJECT_NAME}
    libYson.a
    phidget22)
