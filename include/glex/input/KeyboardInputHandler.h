#pragma once
#include "glex/input/InputHandler.h"
#include "glex/input/KeyCode.h"
#include "glex/common/gl.h"

#include <functional>

#ifdef DREAMCAST
#include <dc/maple.h>
#endif

/*
 * Current limitations:
 *   - Only supports a single keyboard
 *   - Only supports US keyboard layouts
 *   - Deals with raw key codes
 */

typedef std::function<void(KeyCode)> KeyboardCallback;

class KeyboardInputHandler: public InputHandler {
    public:
        virtual InputType type() { return InputType::Keyboard; }

#ifdef GLFW
        virtual void added(GLFWwindow *window);
        virtual void removed(GLFWwindow *window);
#else
        virtual void added();
        virtual void removed();
#endif
        virtual void poll();

        void registerCallback(KeyboardCallback callback) { _callback = callback; }
        void unregisterCallback() { _callback = NULL; }
        void keyPressed(int keyValue);
    private:
        KeyboardCallback _callback;

#ifdef DREAMCAST
        maple_device_t* _mapleDevice;
#endif
};
