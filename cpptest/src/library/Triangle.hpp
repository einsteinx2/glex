#pragma once
#include "../common_gl.h"

class Triangle {
    public:
        Triangle();
        void draw();
    private:
        GLint _list;
};
