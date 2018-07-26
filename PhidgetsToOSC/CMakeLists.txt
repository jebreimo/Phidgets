cmake_minimum_required(VERSION 3.9)
project(PhidgetsToOSC C)

set(CMAKE_C_STANDARD 99)

include_directories(/Users/janeb/Repositories/Phidgets/Phidgets)

add_executable(PhidgetsToOSC
    main.c
    tinyosc.c
    tinyosc.h)
