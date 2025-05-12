cmake_minimum_required(VERSION 3.10)
project(usb2bt)

# Set the C++ standard to use
set(CMAKE_CXX_STANDARD 17)

# Include the source directory for headers
include_directories(src)

# Create a library target for reusable components
add_library(usb2bt_lib
    src/usb_reader.cpp
    src/device_manager.cpp
    src/xinput_mapper.cpp
)

# Link the library to the main executable
add_executable(usb2bt
    src/main.cpp
)

# Ensure the library is linked to the executable
target_link_libraries(usb2bt usb2bt_lib)
