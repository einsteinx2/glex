#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "debug_log.h"
#include "Wrapper.h"
#include "Triangle.h"
#include "Cube.h"

/* program entry */
int main(int argc, char *argv[])
{
    DEBUG_WRITELN("Application started!");
    Wrapper wrapper;
    wrapper.createWindow();

    // Main loop
    Triangle triangle;
    Cube cube;
    while (!glfwWindowShouldClose(wrapper.window)) {
        wrapper.clear();

        //triangle.draw();
        cube.draw();
        
        wrapper.swapBuffers();
    }

    wrapper.closeWindow();
}