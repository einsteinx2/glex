#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../debug_log.h"
#include "Wrapper.h"
#include "Triangle.h"
#include "Cube.h"
#include "Font.h"

/* program entry */
int main(int argc, char *argv[])
{
    DEBUG_PRINTLN("Application started!");
    Wrapper wrapper;
    wrapper.createWindow();

    // Main loop
    Triangle triangle;
    Cube cube;
    Font arialFont;
    arialFont.scale = 2.0;
    arialFont.createTexture();
    while (!glfwWindowShouldClose(wrapper.window)) {
        // Clear the buffer to draw the prepare frame
        wrapper.clear();

        // Draw the 3d object(s)
        wrapper.reshapeFrustum();
        triangle.draw();
        cube.draw();

        // Draw the text
        glClear(GL_DEPTH_BUFFER_BIT);
        wrapper.reshapeOrtho(arialFont.scale);
        arialFont.printAt(20, 20, L"Hello World !");

        // Swap buffers to display the current frame
        wrapper.swapBuffers();
    }

    wrapper.closeWindow();
}