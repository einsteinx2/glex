#pragma once
#include "glex/common/gl.h"

// Xbox controller style button layout
namespace GamepadButton {
    constexpr uint8_t A          = 0;
    constexpr uint8_t B          = 1;
    constexpr uint8_t X          = 2;
    constexpr uint8_t Y          = 3;
    constexpr uint8_t L_SHOULDER = 4;
    constexpr uint8_t R_SHOULDER = 5;
    constexpr uint8_t BACK       = 6;
    constexpr uint8_t START      = 7;
    constexpr uint8_t GUIDE      = 8;
    constexpr uint8_t L_STICK    = 9;
    constexpr uint8_t R_STICK    = 10;
    constexpr uint8_t DPAD_UP    = 11;
    constexpr uint8_t DPAD_RIGHT = 12;
    constexpr uint8_t DPAD_DOWN  = 13;
    constexpr uint8_t DPAD_LEFT  = 14;
    constexpr uint8_t LAST       = DPAD_LEFT;
};

// Thumb sticks and analog triggers
namespace GamepadAnalog {
    constexpr uint8_t L_STICK_X = 0;
    constexpr uint8_t L_STICK_Y = 1;
    constexpr uint8_t R_STICK_X = 2;
    constexpr uint8_t R_STICK_Y = 3;
    constexpr uint8_t L_TRIGGER = 4;
    constexpr uint8_t R_TRIGGER = 5;
    constexpr uint8_t LAST      = R_TRIGGER;
};

// Button states
namespace GamepadButtonState {
    constexpr uint8_t RELEASED = 0;
    constexpr uint8_t PRESSED  = 1;
};

// Gamepad button and analog stick/trigger states
struct GamepadState {
    // The states of each GamepadButton; values from GamepadButtonState
    unsigned char buttons[15];
    // The states of each GamepadAnalog; values between -1.0 and 1.0 inclusive
    float analog[6];
};
