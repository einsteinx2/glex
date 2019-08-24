#pragma once
#include "../common_gl.h"
#include "Texture.hpp"

class Cube {
    public:
        Cube();
        void draw();
    private:
        GLint _list;
        GLfloat _anglex;
        GLfloat _angley;
        GLfloat _anglez;
        Texture _texture;
};