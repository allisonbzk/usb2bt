#pragma once

struct GamepadState {
    bool buttons[18] = {};
    float lx = 0.0f;
    float ly = 0.0f;
    float rx = 0.0f;
    float ry = 0.0f;
};
