#include "gamepad.h"
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <iostream>
#include <map>
#include <cstring>

static std::string find_device_by_name(const std::string& name) {
    DIR* dir = opendir("/dev/input");
    if (!dir) return "";

    struct dirent* ent;
    while ((ent = readdir(dir)) != nullptr) {
        if (strncmp(ent->d_name, "event", 5) == 0) {
            std::string path = std::string("/dev/input/") + ent->d_name;
            int fd = open(path.c_str(), O_RDONLY);
            if (fd >= 0) {
                char devname[256] = {0};
                ioctl(fd, EVIOCGNAME(sizeof(devname)), devname);
                if (name == devname) {
                    close(fd);
                    closedir(dir);
                    return path;
                }
                close(fd);
            }
        }
    }
    closedir(dir);
    return "";
}

static void print_gamepad_state(const GamepadState& state) {
    const char* btnNames[] = {"A","B","X","Y","LB","RB","LT","RT",
                              "SELECT","START","LS","RS",
                              "UP","DOWN","LEFT","RIGHT","HOME","SHOT"};
    for (int i = 0; i < 18; ++i) {
        if (state.buttons[i]) std::cout << btnNames[i] << " ";
    }
    std::cout << "LX: " << state.lx << " LY: " << state.ly
              << " RX: " << state.rx << " RY: " << state.ry << std::endl;
}

void read_input_loop(const std::string& deviceName) {
    std::string path = find_device_by_name(deviceName);
    if (path.empty()) {
        std::cerr << "Device not found\n";
        return;
    }

    int fd = open(path.c_str(), O_RDONLY);
    if (fd < 0) {
        std::cerr << "Failed to open device\n";
        return;
    }

    GamepadState state = {};
    input_event ev;
    while (read(fd, &ev, sizeof(ev)) > 0) {
        if (ev.type == EV_KEY) {
            switch (ev.code) {
                case BTN_SOUTH: state.buttons[0] = ev.value; break; // A
                case BTN_EAST:  state.buttons[1] = ev.value; break; // B
                case BTN_NORTH: state.buttons[2] = ev.value; break; // X
                case BTN_WEST:  state.buttons[3] = ev.value; break; // Y
                case BTN_TL:    state.buttons[4] = ev.value; break; // LB
                case BTN_TR:    state.buttons[5] = ev.value; break; // RB
                case BTN_TL2:   state.buttons[6] = ev.value; break; // LT
                case BTN_TR2:   state.buttons[7] = ev.value; break; // RT
                case BTN_SELECT: state.buttons[8] = ev.value; break;
                case BTN_START:  state.buttons[9] = ev.value; break;
                case BTN_THUMBL: state.buttons[10] = ev.value; break;
                case BTN_THUMBR: state.buttons[11] = ev.value; break;
                case BTN_DPAD_UP:    state.buttons[12] = ev.value; break;
                case BTN_DPAD_DOWN:  state.buttons[13] = ev.value; break;
                case BTN_DPAD_LEFT:  state.buttons[14] = ev.value; break;
                case BTN_DPAD_RIGHT: state.buttons[15] = ev.value; break;
                case KEY_HOMEPAGE:   state.buttons[16] = ev.value; break;
                case KEY_SYSRQ:      state.buttons[17] = ev.value; break;
            }
            print_gamepad_state(state);
        } else if (ev.type == EV_ABS) {
            switch (ev.code) {
                case ABS_X: state.lx = ev.value / 32767.0f; break;
                case ABS_Y: state.ly = ev.value / 32767.0f; break;
                case ABS_RX: state.rx = ev.value / 32767.0f; break;
                case ABS_RY: state.ry = ev.value / 32767.0f; break;
            }
        }
    }

    close(fd);
}
