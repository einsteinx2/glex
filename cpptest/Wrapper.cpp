#include "Wrapper.h"
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

/* new window size */
void reshape(GLFWwindow* window, int width, int height)
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
    glTranslatef(0.0, 0.0, -20.0);
}

void Wrapper::createWindow() {
    if (!glfwInit()) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit( EXIT_FAILURE );
    }

    glfwWindowHint(GLFW_DEPTH_BITS, 16);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    window = glfwCreateWindow(300, 300, "Fixed Function Triangle", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to open GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Set callback functions
    glfwSetFramebufferSizeCallback(window, reshape);
    glfwSetKeyCallback(window, key);

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    reshape(window, width, height);

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