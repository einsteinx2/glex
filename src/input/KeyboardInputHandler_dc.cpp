#ifdef DREAMCAST

#include "glex/input/KeyboardInputHandler.h"

#include <dc/maple/keyboard.h>

void KeyboardInputHandler::added() {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    kbd_set_queue(1); // Enable global keyboard queue
#pragma GCC diagnostic pop
}

void KeyboardInputHandler::removed() {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    kbd_set_queue(0); // Disable global keyboard queue
#pragma GCC diagnostic pop
}

#endif