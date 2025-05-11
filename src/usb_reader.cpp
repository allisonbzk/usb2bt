#include "usb_reader.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <linux/input.h>
#include <fcntl.h>
#include <sys/ioctl.h>

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

void read_usb_input(const std::string& devicePath, void (*buttonCallback)(int), void (*axisCallback)(int, float)) {
    int fd = open(devicePath.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "Falha ao abrir o dispositivo " << devicePath << "\n";
        return;
    }

    struct input_event ev;
    while (read(fd, &ev, sizeof(struct input_event)) > 0) {
        if (ev.type == EV_KEY) {
            buttonCallback(ev.code);
        } else if (ev.type == EV_ABS) {
            axisCallback(ev.code, ev.value / 32767.0f);  // Normalizando o valor do eixo
        }
    }
    close(fd);
}
