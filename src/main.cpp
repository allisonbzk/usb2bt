#include "device_manager.hpp"
#include "usb_reader.hpp"
#include <iostream>
#include <vector>

void print_button_info(int buttonCode) {
    // Existing implementation
}

void print_axis_info(int axisCode, float value) {
    // Existing implementation
}

int main(int argc, char* argv[]) {
    DeviceManager deviceManager;
    std::vector<InputDeviceInfo> devices = deviceManager.listDevices();
    std::string forceRepickArg = argc > 1 ? argv[1] : "";

    int selectedIndex = -1;
    std::string selectedName;

    if (forceRepickArg == "--repick" || deviceManager.loadSelectedDeviceName().empty()) {
        selectedIndex = deviceManager.selectDeviceInteractively(devices);
        selectedName = devices[selectedIndex].name;
    } else {
        selectedName = deviceManager.loadSelectedDeviceName();
        for (size_t i = 0; i < devices.size(); ++i) {
            if (devices[i].name == selectedName) {
                selectedIndex = i;
                break;
            }
        }
        if (selectedIndex == -1) {
            std::cerr << "Dispositivo salvo não encontrado. Refazendo seleção.\n";
            selectedIndex = deviceManager.selectDeviceInteractively(devices);
            selectedName = devices[selectedIndex].name;
        }
    }

    std::string path = devices[selectedIndex].path;
    std::cout << "Lendo eventos de: " << selectedName << " (" << path << ")\n";
    read_usb_input(path, print_button_info, print_axis_info);
    return 0;
}
