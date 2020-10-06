#pragma once
#include "glex/common/gl.h"
#include "Texture.h"

class Cube {
public:
    Texture* texture = nullptr;

    Cube() {};
    Cube(Texture* texture_) { texture = texture_; }

    void draw();

private:
    GLfloat _anglex = 0.0;
    GLfloat _angley = 0.0;
    GLfloat _anglez = 0.0;    

    void _drawList();
};
