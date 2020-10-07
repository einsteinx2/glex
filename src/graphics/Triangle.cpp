#include "glex/graphics/Triangle.h"
#include "glex/common/log.h"

void Triangle::draw() {
    // Set OpenGL draw settings
    glDisable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();

    // Perform scaling and rotation
    glScalef(scale, scale, 1.0);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);

    _drawList();

    // Unset OpenGL draw settings
    glDisable(GL_BLEND);

    glPopMatrix();
}

void Triangle::_drawList() {
    // Define the vertex arrays 
    float colors[] { 1.0, 0.0, 0.0,
                     0.0, 1.0, 0.0,
                     0.0, 0.0, 1.0 };

    float verts[] = { x,                  y - height, z,   // Bottom Left
                      x + width,          y - height, z,   // Bottom Right
                      x + (width / 2.0f), y,          z }; // Top Center

    // Enable color and vertex array drawing modes
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    // Specify the arrays to use
    glColorPointer(3, GL_FLOAT, 0, colors);
    glVertexPointer(3, GL_FLOAT, 0, verts);

    // Draw the tri
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Disable color and vertex array drawing modes
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}