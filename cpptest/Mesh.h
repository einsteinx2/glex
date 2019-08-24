#pragma once
#include "../common_gl.h"
#include "Texture.h"

class Mesh {
    public:
        Mesh();
        void draw();
    private:
        GLint _list;
        GLfloat _anglex;
        GLfloat _angley;
        GLfloat _anglez;
        Texture _texture;
};