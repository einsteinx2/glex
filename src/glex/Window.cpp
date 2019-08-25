#include "glex/Window.h"
#include "common_gl.h"
#include "debug_log.h"

extern "C" {
    /* change view angle, exit upon ESC */
    #ifdef DREAMCAST
        // TODO: Implement input handling for Dreamcast
    #else
    void key(GLFWwindow* window, int k, int s, int action, int mods) {
        if (action != GLFW_PRESS) {
            return;
        }

        Window* wrapper = (Window*)glfwGetWindowUserPointer(window);

        switch (k) {
        case GLFW_KEY_Z:
            if( mods & GLFW_MOD_SHIFT )
            wrapper->rotationZ -= 5.0;
            else
            wrapper->rotationZ += 5.0;
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        case GLFW_KEY_UP:
            wrapper->rotationX -= 5.0;
            break;
        case GLFW_KEY_DOWN:
            wrapper->rotationX += 5.0;
            break;
        case GLFW_KEY_LEFT:
            wrapper->rotationY -= 5.0;
            break;
        case GLFW_KEY_RIGHT:
            wrapper->rotationY += 5.0;
            break;
        default:
            return;
        }
    }
    #endif

    #ifdef DREAMCAST
    void _reshapeFrustum(int width, int height)
    #else
    void _reshapeFrustum(GLFWwindow* window, int width, int height)
    #endif
    {
        GLfloat h = (GLfloat)height / (GLfloat)width;
        GLfloat xmax, znear, zfar;

        znear = 5.0f;
        zfar  = 30.0f;
        xmax  = znear * 0.5f;

        glViewport(0, 0, (GLint) width, (GLint) height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-xmax, xmax, -xmax * h, xmax * h, znear, zfar);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.5, 0.5, -20.0);
    }

    #ifdef DREAMCAST
    void _reshapeOrtho(int width, int height) 
    #else
    void _reshapeOrtho(GLFWwindow* window, int width, int height) 
    #endif
    {

        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width, 0, height, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0, 0.0, 0.0);
    }
}

void Window::reshapeFrustum() {
    #ifdef DREAMCAST
    {
        _reshapeFrustum(_width, _height);
    }
    #else
    {
        glfwGetFramebufferSize(_window, &_width, &_height);
        _reshapeFrustum(_window, _width, _height);
    }
    #endif
}

void Window::reshapeOrtho(float scale) {
    #ifdef DREAMCAST
    {
        _reshapeOrtho(_width, _height);
        glViewport(0, 0, (int)((float)_width * scale), (int)((float)_height * scale));
    }
    #else
    {
        glfwGetFramebufferSize(_window, &_width, &_height);
        _reshapeOrtho(_window, _width, _height);
        glViewport(0, 0, (int)((float)_width * scale), (int)((float)_height * scale));
    }
    #endif
}

void Window::createWindow(std::string windowName, int width, int height) {
    _windowName = windowName;
    _width = width;
    _height = height;

    #ifdef DREAMCAST
    {
        glKosInit();
        _reshapeFrustum(_width, _height);
    }
    #else
    {
        if (!glfwInit()) {
            fprintf( stderr, "Failed to initialize GLFW\n" );
            exit( EXIT_FAILURE );
        }

        glfwWindowHint(GLFW_DEPTH_BITS, 16);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

        _window = glfwCreateWindow(_width, _height, _windowName.c_str(), NULL, NULL);
        if (!_window) {
            fprintf(stderr, "Failed to open GLFW window\n");
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        // Set callback functions
        glfwSetWindowUserPointer(_window, this);
        glfwSetFramebufferSizeCallback(_window, _reshapeFrustum);
        //glfwSetFramebufferSizeCallback(_window, _reshapeOrtho);
        glfwSetKeyCallback(_window, key);

        // Lock to (probably) 60fps if vsyncEnabled, or unlock framerate
        glfwMakeContextCurrent(_window);
        gladLoadGL(glfwGetProcAddress);
        glfwSwapInterval(vsyncEnabled ? 1 : 0);

        // Setup the frame buffer and view port size
        glfwGetFramebufferSize(_window, &_width, &_height);
        DEBUG_PRINTLN("_width: %d _height: %d", _width, _height);
        _reshapeFrustum(_window, _width, _height);
        //_reshapeOrtho(_window, _width, _height);

        // Handle resizing
        // auto sizeCallback = [this] (GLFWwindow* win, int w, int h) {
        //     reshape(_window, width, height);
        // };
        //printf("reshape pointer: %d", Wrapper::reshape);
        // glfwSetWindowUserPointer(_window, this);
        // auto sizeCallback = [](GLFWwindow* win, int width, int height) {
        //     static_cast<Wrapper*>(glfwGetWindowUserPointer(win))->reshape(width, height);
        // };
        // glfwSetFramebufferSizeCallback(_window, sizeCallback);
    }
    #endif
}

void Window::closeWindow() {
    #ifdef DREAMCAST
    {
        // Not implemented for Dreamcast
    }
    #else
    {
        // Terminate GLFW
        glfwTerminate();

        // Exit program
        exit(EXIT_SUCCESS);
    }
    #endif
}

void Window::clear() {
    // Set the background color
    glClearColor(0.0, 0.0, 0.0, 0.75);

    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::swapBuffers() {
    #ifdef DREAMCAST
    {
        glKosSwapBuffers();
    }
    #else
    {
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }
    #endif
}

int Window::windowShouldClose() {
    #ifdef DREAMCAST
    {
        return 0;
    }
    #else
    {
        return glfwWindowShouldClose(_window);
    }
    #endif
}