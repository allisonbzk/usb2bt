#include "xinput_mapper.hpp"
#include "usb_reader.hpp"
#include <linux/input.h>
#include <iostream>

std::vector<InputDeviceInfo> list_input_devices() {
    std::vector<InputDeviceInfo> devices;

    // Código para listar os dispositivos USB (em Linux)
    for (int i = 0; i < 256; ++i) {
        char path[64];
        snprintf(path, sizeof(path), "/dev/input/event%d", i);
        int fd = open(path, O_RDONLY);
        if (fd == -1) continue;

        char name[256];
        if (ioctl(fd, EVIOCGNAME(sizeof(name)), name) >= 0) {
            devices.push_back({name, path});
        }
        close(fd);
    }
    return devices;
}


// Modify the read_usb_input function
void read_usb_input(const std::string& devicePath, void (*buttonCallback)(int), void (*axisCallback)(int, float)) {
    int fd = open(devicePath.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "Failed to open the device " << devicePath << "\n";
        return;
    }

    struct input_event ev;
    while (read(fd, &ev, sizeof(struct input_event)) > 0) {
        try {
            if (ev.type == EV_KEY) {
                XInputButton xButton = map_usb_to_xinput_button(ev.code);
                buttonCallback((int)xButton);
            } else if (ev.type == EV_ABS) {
                XInputAxis xAxis = map_usb_to_xinput_axis(ev.code);
                axisCallback((int)xAxis, ev.value / 32767.0f); // Normalize axis value
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Unmapped input code: " << ev.code << " (" << e.what() << ")\n";
        }
    }
    close(fd);
}
