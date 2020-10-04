#pragma once
#include "glex/input/InputHandler.h"
#include "glex/common/gl.h"
#include "glex/input/MouseState.h"

#include <functional>
#include <string>

#ifdef DREAMCAST
#include <dc/maple.h>
#endif

/*
 * Current limitations:
 *   - Raw mouse coordinates only, even on PC, as GLEX is console focused. 
 *     So you'll need to manually draw your own mouse cursor and move it based on the mouse coordinates.
 *   - Scroll wheel not implemented yet
 *   - Only 3 mouse buttons are supported (should be enough for anyone, just like 640K RAM...)
 *   - Only a single mouse is supported (would you really need more?)
 */

typedef std::function<void(MouseState)> MouseRawStateCallback;

class MouseInputHandler: public InputHandler {
    public:
        virtual InputType type() { return InputType::Mouse; }

#ifdef GLFW
        virtual void added(GLFWwindow *window);
        virtual void removed(GLFWwindow *window);
#else
        virtual void added();
        virtual void removed();
#endif
        virtual void poll();

        void registerRawStateCallback(MouseRawStateCallback rawCallback) { _rawCallback = rawCallback; }
        void unregisterCallback() { _rawCallback = NULL; }

#ifdef GLFW
        void mouseMoved(double xpos, double ypos);
        void mouseScrolled(double xoffset, double yoffset);
        void mouseButtonPressed(int button, int action, int mods);
#endif

        MouseState getCurrentState() { return _currentState; }
    private:
        MouseRawStateCallback _rawCallback = NULL;
        MouseState _currentState;
        bool _mouseMoved = false;
        bool _mouseScrolled = false;
        bool _mouseButtonChanged = false;

#ifdef GLFW
        double _lastX = 0.0;
        double _lastY = 0.0;
#else
        maple_device_t* _mapleDevice = nullptr;
#endif
};