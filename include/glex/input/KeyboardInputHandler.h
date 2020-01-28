#pragma once
#include "glex/input/InputHandler.h"
#include "glex/common/gl.h"

#include <functional>

#ifdef GLFW
enum KeyCode {
    Escape = GLFW_KEY_ESCAPE
};
#endif
#ifdef DREAMCAST
enum KeyCode {
    Escape = 27
};
#endif

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
        void registerCallback(KeyboardCallback callback) { _callback = callback; }
        void unregisterCallback() { _callback = NULL; }

        void keyPressed(int keyValue);
    private:
        KeyboardCallback _callback;
};
