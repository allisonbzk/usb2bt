#ifndef USB_READER_HPP
#define USB_READER_HPP

#include "input_device_info.hpp"
#include <vector>
#include <string>

std::vector<InputDeviceInfo> list_input_devices();
void read_usb_input(const std::string& devicePath, void (*buttonCallback)(int), void (*axisCallback)(int, float));

#endif // USB_READER_HPP
