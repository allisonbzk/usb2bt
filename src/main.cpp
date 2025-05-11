// This is a simplified draft sketch of what the full implementation will look like. // It retains all existing logic and adds uinput virtual device setup and HID gamepad emulation.

#include <iostream>
#include <fstream>
#include <map>
#include <linux/uinput.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <thread>
#include <cstring>
#include <dirent.h>

#define CONFIG_FILE "selected_device.txt" 
#define MAPPING_FILE_PREFIX "mapping_"

std::string get_device_list() { std::string path = "/dev/input"; std::string result; int i = 0; for (const auto & entry : std::filesystem::directory_iterator(path)) { if (entry.path().string().find("event") != std::string::npos) { std::string dev = entry.path().string(); int fd = open(dev.c_str(), O_RDONLY); if (fd >= 0) { char name[256] = ""; ioctl(fd, EVIOCGNAME(sizeof(name)), name); close(fd); result += std::to_string(i++) + ": [" + dev + "] " + name + "\n"; } } } return result; }

std::string select_input_device() { std::string list = get_device_list(); std::cout << "Available input devices:\n" << list; std::cout << "Enter the number of the device: "; int num; std::cin >> num;

int i = 0;
for (const auto & entry : std::filesystem::directory_iterator("/dev/input")) {
    if (entry.path().string().find("event") != std::string::npos) {
        if (i == num) {
            return entry.path().string();
        }
        i++;
    }
}
return "";

}

void save_selected_device(const std::string& dev) { std::ofstream out(CONFIG_FILE); out << dev; out.close(); }

std::string load_selected_device() { std::ifstream in(CONFIG_FILE); std::string dev; std::getline(in, dev); return dev; }

int setup_uinput_device() { int uinput_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK); if (uinput_fd < 0) return -1;

ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY);
ioctl(uinput_fd, UI_SET_EVBIT, EV_ABS);

for (int i = 0; i < 256; ++i) ioctl(uinput_fd, UI_SET_KEYBIT, i);

ioctl(uinput_fd, UI_SET_ABSBIT, ABS_X);
ioctl(uinput_fd, UI_SET_ABSBIT, ABS_Y);
ioctl(uinput_fd, UI_SET_ABSBIT, ABS_RX);
ioctl(uinput_fd, UI_SET_ABSBIT, ABS_RY);
ioctl(uinput_fd, UI_SET_ABSBIT, ABS_Z);     // triggers
ioctl(uinput_fd, UI_SET_ABSBIT, ABS_RZ);
ioctl(uinput_fd, UI_SET_ABSBIT, ABS_HAT0X); // dpad
ioctl(uinput_fd, UI_SET_ABSBIT, ABS_HAT0Y);

struct uinput_user_dev uidev = {};
snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "BT Virtual Gamepad");
uidev.id.bustype = BUS_USB;
uidev.id.vendor  = 0x1234;
uidev.id.product = 0x5678;
uidev.id.version = 1;

write(uinput_fd, &uidev, sizeof(uidev));
ioctl(uinput_fd, UI_DEV_CREATE);
return uinput_fd;

}

void emit_event(int fd, int type, int code, int val) { struct input_event ev = {0}; gettimeofday(&ev.time, NULL); ev.type = type; ev.code = code; ev.value = val; write(fd, &ev, sizeof(ev)); }

void sync_event(int fd) { emit_event(fd, EV_SYN, SYN_REPORT, 0); }

void forward_input_events(const std::string& input_dev, int uinput_fd) { int fd = open(input_dev.c_str(), O_RDONLY); if (fd < 0) return;

struct input_event ev;
while (true) {
    if (read(fd, &ev, sizeof(ev)) == sizeof(ev)) {
        // Forward keys and axes
        if (ev.type == EV_KEY || ev.type == EV_ABS) {
            std::cout << "type: " << ev.type << " code: " << ev.code << " value: " << ev.value << "\n";
            emit_event(uinput_fd, ev.type, ev.code, ev.value);
            sync_event(uinput_fd);
        }
    }
}
close(fd);

}

int main(int argc, char* argv[]) { bool force_select = false; if (argc > 1 && std::string(argv[1]) == "--repick") { force_select = true; }

std::string selected_device;
if (!force_select && std::filesystem::exists(CONFIG_FILE)) {
    selected_device = load_selected_device();
} else {
    selected_device = select_input_device();
    save_selected_device(selected_device);
}

std::cout << "Using device: " << selected_device << "\n";
int uinput_fd = setup_uinput_device();
if (uinput_fd < 0) {
    std::cerr << "Failed to set up uinput device\n";
    return 1;
}

forward_input_events(selected_device, uinput_fd);
return 0;

}


