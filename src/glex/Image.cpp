#include "glex/Image.h"
#include "debug_log.h"

#define IMMEDIATE_MODE true

int Image::width() {
    return _texture == NULL ? 0 : _texture->width;
}

int Image::height() {
    return _texture == NULL ? 0 : _texture->height;
}

Image::Image(Texture* texture, GLfloat scale) {
    _texture = texture;
    _scale = scale;
}

void Image::draw(float x, float y, float width, float height, float windowScale) {
    // Set OpenGL draw settings
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();

    // Perform scaling and rotation
    glScalef(_scale, _scale, 1.0);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);

    _drawList(x, y, width, height, windowScale);

    // Unset OpenGL draw settings
    glDisable(GL_BLEND);

    glPopMatrix();
}

void Image::_drawList(float x, float y, float w, float h, float ws) {
    // Enable texture
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _texture->id);

    // Draw the image
#if IMMEDIATE_MODE
    glBegin(GL_TRIANGLES);
    glColor4f(1.0, 1.0, 1.0, 1.0);

    x *= (1.0 / ws);
    y *= (1.0 / ws);

    // Left triangle
    glTexCoord2f(0, 1); glVertex2f(ws * x,     ws * y  );    // Top Left
    glTexCoord2f(0, 0); glVertex2f(ws * x,     ws * y - h);  // Bottom Left
    glTexCoord2f(1, 0); glVertex2f(ws * x + w, ws * y - h);  // Buttom Right

    // Right triangle
    glTexCoord2f(0, 1); glVertex2f(ws * x,     ws * y  );    // Top Left
    glTexCoord2f(1, 0); glVertex2f(ws * x + w, ws * y - h);  // Bottom Right
    glTexCoord2f(1, 1); glVertex2f(ws * x + w, ws * y  );    // Top Right

    glEnd();
#else

#endif

    glDisable(GL_TEXTURE_2D);
}