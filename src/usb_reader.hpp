#ifndef USB_READER_HPP
#define USB_READER_HPP

#include <vector>
#include <string>

struct InputDeviceInfo {
    std::string name;
    std::string path;
};

std::vector<InputDeviceInfo> list_input_devices();
void read_usb_input(const std::string& devicePath, void (*buttonCallback)(int), void (*axisCallback)(int, float));

#endif // USB_READER_HPP
