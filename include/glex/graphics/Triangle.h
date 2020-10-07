#pragma once
#include "glex/common/gl.h"

class Triangle {
public:
    float x = 0;
    float y = 0;
    float z = 0;
    float width = 0;
    float height = 0;
    float windowScale = 1;

    GLfloat rotationX = 0;
    GLfloat rotationY = 0;
    GLfloat scale = 1;

    Triangle() {};
    Triangle(float x_, float y_, float z_, float width_, float height_, float windowScale_, float scale_ = 1.0) {
        x = x_; y = y_; z = z_; width = width_; height = height_, windowScale = windowScale_; scale = scale_;
    };
    
    void draw();
    
private:
    void _drawList();
};
