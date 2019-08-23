#pragma once
#include "../common_gl.h"

class Cube {
    public:
        Cube();
        void draw();
    private:
        GLint _list;
        GLfloat _anglex;
        GLfloat _angley;
        GLfloat _anglez;
};