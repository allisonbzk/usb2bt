#ifndef XINPUT_MAPPER_HPP
#define XINPUT_MAPPER_HPP

#include <map>
#include <string>

// Define the XInput button and axis codes
enum class XInputButton {
    A, B, X, Y, LB, RB, BACK, START, LS, RS, DPAD_UP, DPAD_DOWN, DPAD_LEFT, DPAD_RIGHT
};

enum class XInputAxis {
    LX, LY, RX, RY, LT, RT
};

// Function to map USB button codes to XInput buttons
XInputButton map_usb_to_xinput_button(int usbCode);

// Function to map USB axis codes to XInput axes
XInputAxis map_usb_to_xinput_axis(int usbAxisCode);

#endif // XINPUT_MAPPER_HPP