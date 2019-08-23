#include "Wrapper.h"
#include "../debug_log.h"

#include <stdlib.h>
#include <stdio.h>

/* change view angle, exit upon ESC */
void key(GLFWwindow* window, int k, int s, int action, int mods) {
    if (action != GLFW_PRESS) {
        return;
    }

    switch (k) {
    //   case GLFW_KEY_Z:
    //     if( mods & GLFW_MOD_SHIFT )
    //       view_rotz -= 5.0;
    //     else
    //       view_rotz += 5.0;
    //     break;
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    break;
    //   case GLFW_KEY_UP:
    //     view_rotx += 5.0;
    //     break;
    //   case GLFW_KEY_DOWN:
    //     view_rotx -= 5.0;
    //     break;
    //   case GLFW_KEY_LEFT:
    //     view_roty += 5.0;
    //     break;
    //   case GLFW_KEY_RIGHT:
    //     view_roty -= 5.0;
    //     break;
    default:
        return;
    }
}

void _reshapeFrustum(GLFWwindow* window, int width, int height)
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

void _reshapeOrtho(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, 0.0);
}

void Wrapper::reshapeFrustum() {
    glfwGetFramebufferSize(window, &_width, &_height);
    _reshapeFrustum(window, _width, _height);
}

void Wrapper::reshapeOrtho(float scale) {
    glfwGetFramebufferSize(window, &_width, &_height);
    _reshapeOrtho(window, _width, _height);
    glViewport(0, 0, (int)((float)_width * scale), (int)((float)_height * scale));
}

void Wrapper::createWindow() {
    if (!glfwInit()) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit( EXIT_FAILURE );
    }

    glfwWindowHint(GLFW_DEPTH_BITS, 16);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    window = glfwCreateWindow(640, 480, "Fixed Function Triangle", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to open GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Set callback functions
    glfwSetFramebufferSizeCallback(window, _reshapeFrustum);
    //glfwSetFramebufferSizeCallback(window, _reshapeOrtho);
    glfwSetKeyCallback(window, key);

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    glfwGetFramebufferSize(window, &_width, &_height);
    DEBUG_PRINTLN("_width: %d _height: %d", _width, _height);
    _reshapeFrustum(window, _width, _height);
    //_reshapeOrtho(window, _width, _height);

    // Handle resizing
    // auto sizeCallback = [this] (GLFWwindow* win, int w, int h) {
    //     reshape(window, width, height);
    // };
    //printf("reshape pointer: %d", Wrapper::reshape);
    // glfwSetWindowUserPointer(window, this);
    // auto sizeCallback = [](GLFWwindow* win, int width, int height) {
    //     static_cast<Wrapper*>(glfwGetWindowUserPointer(win))->reshape(width, height);
    // };
    // glfwSetFramebufferSizeCallback(window, sizeCallback);
}

void Wrapper::closeWindow() {
    // Terminate GLFW
    glfwTerminate();

    // Exit program
    exit(EXIT_SUCCESS);
}

void Wrapper::clear() {
    // Set the background color
    glClearColor(0.0, 0.0, 0.0, 0.75);

    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Wrapper::swapBuffers() {
    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
}