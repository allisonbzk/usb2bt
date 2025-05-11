cmake_minimum_required(VERSION 3.10)
project(usb2bt)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(usb2bt
    src/main.cpp
)
