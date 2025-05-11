cmake_minimum_required(VERSION 3.10)
project(bt_gamepad)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(bt_gamepad
    src/main.cpp
    src/usb_reader.cpp
)
