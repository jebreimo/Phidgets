cmake_minimum_required(VERSION 3.9)
project(Phidget22 C)

set(CMAKE_C_STANDARD 99)

add_library(Phidget22 STATIC
    phidget22.h
    phidget22.c)
