#pragma once
#include "../common_gl.h"
#include "Texture.h"

class Cube {
    public:
        Cube();
        void draw();
    private:
        GLfloat _anglex;
        GLfloat _angley;
        GLfloat _anglez;
        Texture _texture;
        void _drawList();
};
