#pragma once
#include "glex/common/gl.h"

// Button states
namespace MouseButtonState {
    constexpr bool RELEASED = 0;
    constexpr bool PRESSED  = 1;
};

// Mouse movement deltas and button states
struct MouseState {
    double posX         = 0.0;
    double posY         = 0.0;
    double posDeltaX    = 0.0;
    double posDeltaY    = 0.0;
    double scrollDeltaX = 0.0;
    double scrollDeltaY = 0.0;
    bool leftButton     = 0;
    bool centerButton   = 0;
    bool rightButton    = 0;
};

bool operator == (const MouseState &lhs, const MouseState &rhs);
bool operator != (const MouseState &lhs, const MouseState &rhs);
