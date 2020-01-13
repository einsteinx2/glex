#pragma once
#include "../common_gl.h"
#include "Texture.h"

class Image {
public:
    GLfloat rotationX;
    GLfloat rotationY;
    GLfloat scale;

    int width();
    int height();

    Image(Texture* texture, GLfloat scale = 1.0);
    void draw(float x, float y, float width, float height, float windowScale = 1.0);

private:
    Texture* _texture;
    GLfloat _scale;

    void _drawList(float x, float y, float w, float h, float ws);
};
