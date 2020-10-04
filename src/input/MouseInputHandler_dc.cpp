#ifdef DREAMCAST

#include "glex/input/MouseInputHandler.h"
#include "glex/common/log.h"

#include <dc/maple/mouse.h>
#include <dc/maple.h>

void MouseInputHandler::added() {
    // Get the first connected mouse
    // TODO: Support more than one mouse
    // TODO: Support plugging in a mouse after the DC boots
    _mapleDevice = maple_enum_type(0, MAPLE_FUNC_MOUSE);
    if (_mapleDevice == NULL) {
        ERROR_PRINTLN("Error: MouseInputHandler failed to find mouse device, no mouse connected");
        return;
    }

    DEBUG_PRINTLN("MouseInputHandler found mouse device plugged into port: %d", _mapleDevice->port);
}

void MouseInputHandler::removed() {
    // TODO: Need to free() this first?
    _mapleDevice = NULL;
}

void MouseInputHandler::poll() {
    if (_mapleDevice != NULL) {
        mouse_state_t *mouseState = (mouse_state_t *)maple_dev_status(_mapleDevice);
        if (mouseState != NULL) {
            // Remember previous state for comparison
            MouseState prevState = _currentState;

            // Process all mouse changes
            _currentState.posX        += mouseState->dx;
            _currentState.posY        += mouseState->dy;
            _currentState.posDeltaX    = mouseState->dx;
            _currentState.posDeltaY    = mouseState->dy;
            _currentState.scrollDeltaX = mouseState->dz;
            _currentState.leftButton   = mouseState->buttons & MOUSE_LEFTBUTTON;
            _currentState.rightButton  = mouseState->buttons & MOUSE_RIGHTBUTTON;
            _currentState.centerButton = mouseState->buttons & MOUSE_SIDEBUTTON;

            // Check for changes
            if (_rawCallback != NULL && prevState != _currentState) {
                _rawCallback(_currentState);
            }
        }
        
    }
}

#endif