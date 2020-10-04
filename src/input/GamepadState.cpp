#include "glex/input/GamepadState.h"

bool operator == (const GamepadState &lhs, const GamepadState &rhs) {
    for (int i = 0; i <= GamepadButton::LAST; i++) {
        if (lhs.buttons[i] != rhs.buttons[i]) {
            return false;
        }
    }
    for (int i = 0; i <= GamepadAnalog::LAST; i++) {
        if (lhs.analog[i] != rhs.analog[i]) {
            return false;
        }
    }
    return true;
}

bool operator != (const GamepadState &lhs, const GamepadState &rhs) {
    return !(lhs == rhs);
}
