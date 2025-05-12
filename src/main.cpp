#include "usb_reader.hpp"
#include "device_manager.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <filesystem>

std::string configPath = "/data/data/com.termux/files/home/.config/bt-gamepad/";
std::string selectedDeviceFile = configPath + "selected_device.txt";

std::map<int, std::string> buttonMapping = {
    {305, "A"}, {304, "B"}, {308, "X"}, {307, "Y"},
    {310, "LB"}, {311, "RB"}, {312, "LT"}, {313, "RT"},
    {314, "Back"}, {315, "Start"}, {316, "Home"},
    {317, "LS"}, {318, "RS"},
    {114, "Capture(114)"}, {116, "Capture(116)"}
};

// Adds mapping for analog axis values
std::map<int, std::string> axisMapping = {
    {0, "LX"}, {1, "LY"}, {2, "RX"}, {5, "RY"}, {17, "DPad Up/Down"}, {16, "DPad Left/Right"}
};

void print_button_info(int buttonCode) {
    auto it = buttonMapping.find(buttonCode);
    if (it != buttonMapping.end()) {
        std::cout << "Botão pressionado: " << it->second << " (Código: " << buttonCode << ")\n";
    } else {
        std::cout << "Botão desconhecido (Código: " << buttonCode << ")\n";
    }
}

void print_axis_info(int axisCode, float value) {
    auto it = axisMapping.find(axisCode);
    if (it != axisMapping.end()) {
        std::cout << "Eixo " << it->second << " (Código: " << axisCode << "): " << value << "\n";
    } else {
        std::cout << "Eixo desconhecido (Código: " << axisCode << "): " << value << "\n";
    }
}

int main(int argc, char* argv[]) {
    DeviceManager deviceManager; // Use the DeviceManager class
    std::vector<InputDeviceInfo> devices = deviceManager.listDevices(); // Updated to use listDevices()
    std::string forceRepickArg = argc > 1 ? argv[1] : "";

    int selectedIndex = -1;
    std::string selectedName;

    if (forceRepickArg == "--repick" || deviceManager.loadSelectedDeviceName().empty()) { // Updated to use loadSelectedDeviceName()
        selectedIndex = deviceManager.selectDeviceInteractively(devices); // Updated to use selectDeviceInteractively()
        selectedName = devices[selectedIndex].name;
        deviceManager.saveSelectedDeviceName(selectedName); // Save after selection
    } else {
        selectedName = deviceManager.loadSelectedDeviceName();
        for (size_t i = 0; i < devices.size(); ++i) {
            if (devices[i].name == selectedName) {
                selectedIndex = i;
                break;
            }
        }
        if (selectedIndex == -1) {
            std::cerr << "Dispositivo salvo não encontrado. Refaça a seleção.\n";
            selectedIndex = deviceManager.selectDeviceInteractively(devices);
            selectedName = devices[selectedIndex].name;
            deviceManager.saveSelectedDeviceName(selectedName); // Save after reselection
        }
    }

    std::string path = devices[selectedIndex].path;
    std::cout << "Lendo eventos de: " << selectedName << " (" << path << ")\n";
    read_usb_input(path, print_button_info, print_axis_info);
    return 0;
}
