#include "glex/input/KeyboardInputHandler.h"

void KeyboardInputHandler::keyPressed(int keyValue) {
    if (_callback != NULL) {
        auto keyCode = static_cast<KeyCode>(keyValue);
        _callback(keyCode);
    }
}