#pragma once
#include "glex/common/gl.h"
#include "Texture.h"

class Cube {
public:
    Cube();
    void draw();

private:
    GLfloat _anglex = 0.0;
    GLfloat _angley = 0.0;
    GLfloat _anglez = 0.0;
    Texture _texture;
    
    void _drawList();
};
