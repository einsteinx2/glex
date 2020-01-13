#include "glex/Window.h"
#include "common_gl.h"
#include "debug_log.h"

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
    //Window* wrapper = (Window*)glfwGetWindowUserPointer(window);
    //glViewport(0, 0, width * wrapper->screenScale, height * wrapper->screenScale);
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
    //DEBUG_PRINTLN("_reshapeOrtho   width: %d  height: %d", width, height);
    //Window* wrapper = (Window*)glfwGetWindowUserPointer(window);
    //glViewport(0, 0, width * wrapper->screenScale, height * wrapper->screenScale);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, 0.0);
}

#ifdef DREAMCAST
void _sizeCallback(int width, int height)
#else
void _sizeCallback(GLFWwindow* window, int width, int height)
#endif
{
    //printf("sizeCallback  width: %d  height %d", width, height);
#ifdef DREAMCAST
    _reshapeFrustum(width, height);
    _reshapeOrtho(width, height);
#else
    Window* wrapper = (Window*)glfwGetWindowUserPointer(window);
    wrapper->_updateWindowSize();
    _reshapeFrustum(window, width, height);
    _reshapeOrtho(window, width, height);
#endif
}

void Window::reshapeFrustum() {
#ifdef DREAMCAST
    _reshapeFrustum(_width, _height);
#else
    _reshapeFrustum(_window, _width * screenScale, _height * screenScale);
#endif
}

void Window::reshapeOrtho(float scale) {
#ifdef DREAMCAST
    _reshapeOrtho(_width, _height);
#else
    _reshapeOrtho(_window, _width, _height);
#endif
    glViewport(0, 0, (int)((float)_width * scale * screenScale), (int)((float)_height * screenScale));
}

void Window::clear() {
    // Set the background color
    glClearColor(0.0, 0.0, 0.0, 0.75);

    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::_updateWindowSize() {
#ifndef DREAMCAST
    glfwGetWindowSize(_window, &_width, &_height);
#endif
}

/*
 * Override in subclasses
 */

void createWindow(std::string windowName, int width, int height) {}
void closeWindow() {}
void swapBuffers() {}
int windowShouldClose() { return 0; }