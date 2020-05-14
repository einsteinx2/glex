#include "glex/common/log.h"
#include "glex/common/gl.h"
#include "glex/Triangle.h"
#include "glex/Cube.h"
#include "glex/Mesh.h"
#include "glex/Image.h"
#include "glex/Text.h"
#include "glex/MeshLoader.h"

#include <stdio.h>
#include <cstdlib>
#include <string>
#include <chrono>

std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
uint16_t timeDiff = 0;
uint16_t nbFrames = 0;
float frameTime = 0.0;
uint16_t fps = 0;

bool vsyncEnabled = true; 
int _windowWidth = 0;
int _windowHeight = 0;
std::string _windowName = "";
float screenScale = 1.0;
#ifdef GLFW
    GLFWwindow* _window;
#endif

void _reshapeFrustum(int width, int height) {
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

void reshapeFrustum() {
    _reshapeFrustum((int)((float)_windowWidth * screenScale), (int)((float)_windowHeight * screenScale));
}

void _reshapeOrtho(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -100.1, 100.1); // Added .1 to znear and zfar to allow using the full -100 - 100 range
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, 0.0);
}

void reshapeOrtho(float scale) {
    _reshapeOrtho(_windowWidth, _windowHeight);
    glViewport(0, 0, (int)((float)_windowWidth * scale * screenScale), (int)((float)_windowHeight * screenScale));
}

#ifdef GLFW
void _updateWindowSize() {
    glfwGetWindowSize(_window, &_windowWidth, &_windowHeight);
}

void _sizeCallback(GLFWwindow* window, int width, int height) {
    DEBUG_PRINTLN("entered _sizeCallback");
    _updateWindowSize();
    _reshapeFrustum(width, height);
    _reshapeOrtho(width, height);
}
#endif

void createWindow(std::string windowName, int width, int height) {
#ifdef GLFW
    _windowName = windowName;
    _windowWidth = width;
    _windowHeight = height;

    if (!glfwInit()) {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit( EXIT_FAILURE );
    }

    glfwWindowHint(GLFW_DEPTH_BITS, 16);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    _window = glfwCreateWindow(width, height, _windowName.c_str(), NULL, NULL);
    if (!_window) {
        fprintf(stderr, "Failed to open GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Set callback functions
    glfwSetFramebufferSizeCallback(_window, _sizeCallback);

    // Lock to (probably) 60fps if vsyncEnabled, or unlock framerate
    glfwMakeContextCurrent(_window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(vsyncEnabled ? 1 : 0);

    // Setup the frame buffer and view port size
    _reshapeFrustum(width, height);
    //_reshapeOrtho(width, height); // Don't seem to need to set ortho size here...

    // Detect screen scale by comparing window size to framebuffer size
    int frameBufferWidth;
    glfwGetFramebufferSize(_window, &frameBufferWidth, NULL);
    screenScale = (float)frameBufferWidth / (float)width;
    DEBUG_PRINTLN("screen scale detected: %f", screenScale);
#else
    _windowName = windowName;
    _windowWidth = width;
    _windowHeight = height;

    glKosInit();
    _reshapeFrustum(width, height);
#endif
}

void handleInput() {
#ifdef GLFW
    // Calls all GLFW event handler callback functions
    glfwPollEvents();
#endif
}

void clear() {
    // Set the background color
    glClearColor(0.0, 0.0, 0.0, 0.75);

    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void swapBuffers() {
#ifdef GLFW
    glfwSwapBuffers(_window);
#else
    glKosSwapBuffers();
#endif
}

int windowShouldClose() {
#ifdef GLFW
    return glfwWindowShouldClose(_window);
#else
    return false;
#endif
}

void closeWindow() {
#ifdef GLFW
    // Terminate GLFW
    glfwTerminate();

    // Exit program
    exit(EXIT_SUCCESS);
#else
    exit(EXIT_SUCCESS);
#endif
}

/* program entry */
int main(int argc, char *argv[]) {
    DEBUG_PRINTLN("Application started!");
    DEBUG_PRINTLN("creating window");
    createWindow("GLEX Playground", 640, 480);

    DEBUG_PRINTLN("loading grayBrickTexture");
    Texture grayBrickTexture;
    grayBrickTexture.loadRGB("images/gray_brick_512.jpg");
    DEBUG_PRINTLN("creating grayBrickImage");
    Image grayBrickImage(&grayBrickTexture, 0, (float)_windowHeight, Image::Z_BACKGROUND, _windowWidth, (float)_windowHeight, screenScale);

    DEBUG_PRINTLN("loading woodTexture");
    Texture woodTexture;
    woodTexture.loadRGB("images/wood1.bmp");
    DEBUG_PRINTLN("creating woodImage");
    Image woodImage(&woodTexture, 250, 420, 10, Image::Z_HUD, 100, screenScale);

    DEBUG_PRINTLN("loading houseMesh");
    MeshData *houseMesh = MeshLoader::loadObjMesh("meshes/house.obj");
    DEBUG_PRINTLN("loading houseTexture");
    Texture houseTexture;
    houseTexture.loadRGBA("images/house_512.png");
    DEBUG_PRINTLN("creating mesh");
    Mesh mesh(houseMesh, &houseTexture, 0.3f);

    DEBUG_PRINTLN("loading fontFace");
    FontColor darkBlue{21, 1, 148};
    FontFace fontFace = screenScale > 1.0 ? FontFace::arial_28 : FontFace::arial_16;
    DEBUG_PRINTLN("creating fpsCounter");
    Text fpsCounter(fontFace, "", darkBlue, 20, 20, Image::Z_HUD, screenScale);
    DEBUG_PRINTLN("creating fpsCounter texture");
    fpsCounter.createTexture();

    // Main loop
    DEBUG_PRINTLN("entering main loop");
    while (!windowShouldClose()) {
        DEBUG_PRINTLN("measuring speed");
        // Measure speed
        nbFrames++;
        currentTime = std::chrono::steady_clock::now();
        timeDiff = (uint16_t)std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
        if (timeDiff >= 1000) {
            frameTime = float(timeDiff) / float(nbFrames);
            fps = nbFrames;
            nbFrames = 0;
            lastTime = currentTime;
        }

        // Handle input
        handleInput();

        // Clear the buffer to draw the prepare frame
        DEBUG_PRINTLN("clearing buffer");
        clear();

        // Draw the background image
        DEBUG_PRINTLN("calling reshapeOrtho");
        reshapeOrtho(1.0);
        DEBUG_PRINTLN("drawing grayBrickImage"); 
        grayBrickImage.draw();

        // Draw the 3d rotating house
        DEBUG_PRINTLN("calling reshapeFrustum");
        reshapeFrustum();
        DEBUG_PRINTLN("drawing mesh");
        mesh.draw();
        mesh.rotationX += 0.75;
        mesh.rotationY += 0.75;
        mesh.rotationZ += 0.75;

        // Draw the foreground 2d image
        DEBUG_PRINTLN("calling reshapeOrtho");
        reshapeOrtho(1.0);
        DEBUG_PRINTLN("drawing woodImage");
        woodImage.draw();

        // Draw the FPS counter HUD text
        DEBUG_PRINTLN("calling reshapeOrtho");
        reshapeOrtho(fpsCounter.scale);
        DEBUG_PRINTLN("drawing fpsCounter");
        static char outputString[50];
        // NOTE: Due to an old GCC bug, we must manually cast floats to double in order to use %f without a warning 
        snprintf(&outputString[0], 50, "frame time: %.2f ms  fps: %.2f", (double)frameTime, (double)fps);
        fpsCounter.text = outputString;
        fpsCounter.draw();

        // Swap buffers to display the current frame
        DEBUG_PRINTLN("swapping buffers");
        swapBuffers();
    }

    DEBUG_PRINTLN("closing window");
    closeWindow();
}
