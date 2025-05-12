#ifndef DEVICE_MANAGER_HPP
#define DEVICE_MANAGER_HPP

#include "input_device_info.hpp"
#include <string>
#include <vector>
#include <map>

class DeviceManager {
private:
    std::string configPath;
    std::string selectedDeviceFile;

public:
    DeviceManager();
    std::vector<InputDeviceInfo> listDevices();
    int selectDeviceInteractively(const std::vector<InputDeviceInfo>& devices);
    std::string loadSelectedDeviceName();
    void saveSelectedDeviceName(const std::string& name);
};

#endif // DEVICE_MANAGER_HPP
