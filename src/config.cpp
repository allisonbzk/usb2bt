#include "config.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

static std::vector<std::string> list_input_devices() {
    std::vector<std::string> result;
    DIR* dir = opendir("/dev/input");
    if (!dir) return result;

    struct dirent* ent;
    while ((ent = readdir(dir)) != nullptr) {
        if (strncmp(ent->d_name, "event", 5) == 0) {
            std::string path = std::string("/dev/input/") + ent->d_name;
            int fd = open(path.c_str(), O_RDONLY);
            if (fd >= 0) {
                char name[256] = {0};
                ioctl(fd, EVIOCGNAME(sizeof(name)), name);
                result.push_back(name);
                close(fd);
            }
        }
    }
    closedir(dir);
    return result;
}

bool load_device_config(std::string& outName, bool forcePick) {
    std::ifstream f("/data/data/.btgamepad_device");
    if (f && !forcePick) {
        std::getline(f, outName);
        return !outName.empty();
    }

    auto devices = list_input_devices();
    if (devices.empty()) return false;

    std::cout << "Pick input device:\n";
    for (size_t i = 0; i < devices.size(); ++i)
        std::cout << i << ": " << devices[i] << "\n";

    int choice = 0;
    std::cin >> choice;
    if (choice < 0 || choice >= (int)devices.size()) return false;

    outName = devices[choice];
    std::ofstream out("/data/data/.btgamepad_device");
    out << outName;
    return true;
}
