#include "glex/Window.h"
#include "glex/common/gl.h"
#include "glex/common/log.h"

// /* change view angle, exit upon ESC */
// #ifdef DREAMCAST
// // TODO: Implement input handling for Dreamcast
// #else
// void key(GLFWwindow* window, int k, int s, int action, int mods) {
//     if (action != GLFW_PRESS) {
//         return;
//     }

//     Window* wrapper = (Window*)glfwGetWindowUserPointer(window);

//     switch (k) {
//     case GLFW_KEY_Z:
//         if( mods & GLFW_MOD_SHIFT )
//         wrapper->rotationZ -= 5.0;
//         else
//         wrapper->rotationZ += 5.0;
//         break;
//     case GLFW_KEY_ESCAPE:
//         glfwSetWindowShouldClose(window, GLFW_TRUE);
//         break;
//     case GLFW_KEY_UP:
//         wrapper->rotationX -= 5.0;
//         break;
//     case GLFW_KEY_DOWN:
//         wrapper->rotationX += 5.0;
//         break;
//     case GLFW_KEY_LEFT:
//         wrapper->rotationY -= 5.0;
//         break;
//     case GLFW_KEY_RIGHT:
//         wrapper->rotationY += 5.0;
//         break;
//     default:
//         return;
//     }
// }
// #endif

void Window::_reshapeFrustum(int width, int height) {
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

void Window::_reshapeOrtho(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -100.1, 100.1); // Added .1 to znear and zfar to allow using the full -100 - 100 range
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, 0.0);
}

void Window::reshapeFrustum() {
    _reshapeFrustum(_width * screenScale, _height * screenScale);
}

void Window::reshapeOrtho(float scale) {
    _reshapeOrtho(_width, _height);
    glViewport(0, 0, (int)((float)_width * scale * screenScale), (int)((float)_height * screenScale));
}

void Window::clear() {
    // Set the background color
    glClearColor(0.0, 0.0, 0.0, 0.75);

    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*
 * Override in subclasses
 */

void createWindow(std::string windowName, int width, int height) {}
void closeWindow() {}
void swapBuffers() {}
int windowShouldClose() { return 0; }