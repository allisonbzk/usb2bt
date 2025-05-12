#include "device_manager.hpp"
#include "usb_reader.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>

DeviceManager::DeviceManager()
    : configPath("/data/data/com.termux/files/home/.config/bt-gamepad/"),
      selectedDeviceFile(configPath + "selected_device.txt") {}

std::vector<InputDeviceInfo> DeviceManager::listDevices() {
    // Reutiliza a função já existente em usb_reader
    return list_input_devices();
}

int DeviceManager::selectDeviceInteractively(const std::vector<InputDeviceInfo>& devices) {
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
    saveSelectedDeviceName(devices[choice].name);
    return choice;
}

std::string DeviceManager::loadSelectedDeviceName() {
    std::ifstream file(selectedDeviceFile);
    if (!file.is_open()) {
        return "";
    }
    std::string name;
    std::getline(file, name);
    return name;
}

void DeviceManager::saveSelectedDeviceName(const std::string& name) {
    std::ofstream out(selectedDeviceFile);
    out << name;
    out.close();
}
