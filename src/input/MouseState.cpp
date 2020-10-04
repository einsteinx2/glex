#include "glex/input/MouseState.h"

bool operator == (const MouseState &lhs, const MouseState &rhs) {
    return lhs.posX         == rhs.posX &&
           lhs.posY         == rhs.posY &&
           lhs.posDeltaX    == rhs.posDeltaX &&
           lhs.posDeltaY    == rhs.posDeltaY &&
           lhs.scrollDeltaX == rhs.scrollDeltaY &&
           lhs.scrollDeltaY == rhs.scrollDeltaY &&
           lhs.leftButton   == rhs.leftButton &&
           lhs.centerButton == rhs.centerButton &&
           lhs.rightButton  == rhs.rightButton;
}

bool operator != (const MouseState &lhs, const MouseState &rhs) {
    return !(lhs == rhs);
}
