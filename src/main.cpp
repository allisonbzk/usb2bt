#include "usb_reader.hpp"
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
    {0, "LX"}, {1, "LY"}, {2, "RX"}, {3, "RY"}, {17, "DPad Up/Down"}, {16, "DPad Left/Right"}
};

int select_device_interactively(const std::vector<InputDeviceInfo> devices) {
    std::cout << "Selecione um dispositivo de entrada:\n";
    for (size_t i = 0; i < devices.size(); ++i) {
        std::cout << i << ": " << devices[i].name << "\n";
    }
    std::cout << "Digite o número correspondente: ";
    int choice;
    std::cin >> choice;
    if (choice < 0 || static_cast<size_t>(choice) >= devices.size()) {
        std::cerr << "Seleção inválida.\n";
        exit(1);
    }
    std::filesystem::create_directories(configPath);
    std::ofstream out(selectedDeviceFile);
    out << devices[choice].name;
    out.close();
    return choice;
}

std::string load_selected_device_name() {
    std::ifstream file(selectedDeviceFile);
    if (!file.is_open()) {
        return "";
    }
    std::string name;
    std::getline(file, name);
    return name;
}

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
    std::vector<InputDeviceInfo> devices = list_input_devices();
    std::string forceRepickArg = argc > 1 ? argv[1] : "";

    int selectedIndex = -1;
    std::string selectedName;

    if (forceRepickArg == "--repick" || load_selected_device_name().empty()) {
        selectedIndex = select_device_interactively(devices);
        selectedName = devices[selectedIndex].name;
    } else {
        selectedName = load_selected_device_name();
        for (size_t i = 0; i < devices.size(); ++i) {
            if (devices[i].name == selectedName) {
                selectedIndex = i;
                break;
            }
        }
        if (selectedIndex == -1) {
            std::cerr << "Dispositivo salvo não encontrado. Refaça a seleção.\n";
            selectedIndex = select_device_interactively(devices);
            selectedName = devices[selectedIndex].name;
        }
    }

    std::string path = devices[selectedIndex].path;
    std::cout << "Lendo eventos de: " << selectedName << " (" << path << ")\n";
    read_usb_input(path, print_button_info, print_axis_info);
    return 0;
}
