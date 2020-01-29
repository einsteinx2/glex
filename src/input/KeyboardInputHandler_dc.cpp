#ifdef DREAMCAST

#include "glex/input/KeyboardInputHandler.h"
#include "glex/common/log.h"

#include <dc/maple/keyboard.h>
#include <dc/maple.h>

void KeyboardInputHandler::added() {
    // Get the first connected keyboard
    // TODO: Support more than one keyboard
    // TODO: Support plugging in a keyboard after the DC boots
    _mapleDevice = maple_enum_type(0, MAPLE_FUNC_KEYBOARD);
    if (_mapleDevice == NULL) {
        ERROR_PRINTLN("Error: KeyboardInputHandler failed to find keyboard device, no keyboard connected");
        return;
    }

    DEBUG_PRINTLN("KeyboardInputHandler found keyboard device plugged into port: %d", _mapleDevice->port);
}

void KeyboardInputHandler::removed() {
    // TODO: Need to free() this first?
    _mapleDevice = NULL;
}

void KeyboardInputHandler::poll() {
    if (_mapleDevice != NULL) {
        // Process all keys in the queue
        int keyValue = kbd_queue_pop(_mapleDevice, 0);
        while (keyValue >= 0) {
            keyPressed(keyValue);
            keyValue = kbd_queue_pop(_mapleDevice, 0);
        }
    }
}

#endif