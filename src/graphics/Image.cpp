#include "glex/graphics/Image.h"
#include "glex/common/log.h"

void Image::draw() {
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

void Image::_drawList() {
    // Enable texture
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    // Define the vertex arrays 
    float verts[] = { x,         y - height, z,   // Bottom Left
                      x + width, y - height, z,   // Bottom Right
                      x + width, y,          z,   // Top Right

                      x,         y - height, z,   // Bottom Left
                      x + width, y,          z,   // Top Right
                      x,         y,          z }; // Top Left

    // TODO: Figure out why it seems like tex coords are flipped vertically compared to vert coords...
    float texCoords[] = { 0, 0,   // Bottom Left
                          1, 0,   // Bottom Right
                          1, 1,   // Top Right

                          0, 0,   // Bottom Left
                          1, 1,   // Top Right 
                          0, 1 }; // Top Left

    // Enable vertex and texture array drawing modes
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // Specify the arrays to use
    glVertexPointer(3, GL_FLOAT, 0, verts);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

    // Draw the quad
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Disable vertex and texture array drawing modes
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glDisable(GL_TEXTURE_2D);
}