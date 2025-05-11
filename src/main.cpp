#include "config.h"
#include "gamepad.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

extern void read_input_loop(const std::string& targetDeviceName);

int main(int argc, char* argv[]) {
    std::string deviceName;
    bool forcePick = (argc > 1 && std::string(argv[1]) == "--repick");

    if (!load_device_config(deviceName, forcePick)) {
        std::cerr << "Failed to load or select input device\n";
        return 1;
    }

    std::cout << "Using device: " << deviceName << std::endl;

    read_input_loop(deviceName);

    return 0;
}
