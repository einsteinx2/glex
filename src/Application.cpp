#include "glex/Application.h"
#include "glex/common/gl.h"
#include "glex/common/log.h"

void Application::_reshapeFrustum(int width, int height) {
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

void Application::_reshapeOrtho(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -100.1, 100.1); // Added .1 to znear and zfar to allow using the full -100 - 100 range
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, 0.0);
}

void Application::reshapeFrustum() {
    _reshapeFrustum(_windowWidth * screenScale, _windowHeight * screenScale);
}

void Application::reshapeOrtho(float scale) {
    _reshapeOrtho(_windowWidth, _windowHeight);
    glViewport(0, 0, (int)((float)_windowWidth * scale * screenScale), (int)((float)_windowHeight * screenScale));
}

void Application::clear() {
    // Set the background color
    glClearColor(0.0, 0.0, 0.0, 0.75);

    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
