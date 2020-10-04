#pragma once
#include "glex/common/gl.h"
#include "Texture.h"

class Cube {
public:
    Cube();
    ~Cube();
    Cube(Texture* texture);
    void draw();
    Texture* getTexture();
    void setTexture(Texture* texture);

private:
    GLfloat _anglex = 0.0;
    GLfloat _angley = 0.0;
    GLfloat _anglez = 0.0;
    Texture* _texture = nullptr;

    void _drawList();
};
