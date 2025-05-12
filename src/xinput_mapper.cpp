#include "xinput_mapper.hpp"
#include <stdexcept>

// Map USB button codes to XInput buttons
XInputButton map_usb_to_xinput_button(int usbCode) {
    static const std::map<int, XInputButton> buttonMap = {
        {305, XInputButton::A}, {304, XInputButton::B}, {308, XInputButton::X}, {307, XInputButton::Y},
        {310, XInputButton::LB}, {311, XInputButton::RB}, {314, XInputButton::BACK}, {315, XInputButton::START},
        {317, XInputButton::LS}, {318, XInputButton::RS}
    };

    auto it = buttonMap.find(usbCode);
    if (it != buttonMap.end()) {
        return it->second;
    }
    throw std::invalid_argument("Unknown USB button code");
}

// Map USB axis codes to XInput axes
XInputAxis map_usb_to_xinput_axis(int usbAxisCode) {
    static const std::map<int, XInputAxis> axisMap = {
        {0, XInputAxis::LX}, {1, XInputAxis::LY}, {2, XInputAxis::RX}, {5, XInputAxis::RY},
        {16, XInputAxis::LT}, {17, XInputAxis::RT}
    };

    auto it = axisMap.find(usbAxisCode);
    if (it != axisMap.end()) {
        return it->second;
    }
    throw std::invalid_argument("Unknown USB axis code");
}