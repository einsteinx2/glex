#include "glex/Image.h"
#include "glex/common/log.h"

#define IMMEDIATE_MODE true

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

    // Draw the image
#if IMMEDIATE_MODE
    glBegin(GL_TRIANGLES);
    glColor4f(1.0, 1.0, 1.0, 1.0);

    // Left triangle
    glTexCoord2f(0, 1); glVertex3f(x,         y           , z);  // Top Left
    glTexCoord2f(0, 0); glVertex3f(x,         y - height  , z);  // Bottom Left
    glTexCoord2f(1, 0); glVertex3f(x + width, y - height  , z);  // Buttom Right

    // Right triangle
    glTexCoord2f(0, 1); glVertex3f(x,         y           , z);  // Top Left
    glTexCoord2f(1, 0); glVertex3f(x + width, y - height  , z);  // Bottom Right
    glTexCoord2f(1, 1); glVertex3f(x + width, y           , z);  // Top Right

    glEnd();
#else

#endif

    glDisable(GL_TEXTURE_2D);
}