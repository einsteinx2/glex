#pragma once
#include "glex/common/gl.h"
#include "Texture.h"

class Image {
public:
    // NOTE: zNear is set to -100 and zFar to 100 in glOrtho, but it seems to work backwards...
    //       -100 is the farthest away (e.g. behind everything) while 100 is the closet (draws over everything)
    
    // Use this as the z value to draw behind everything (e.g. a background image)
    static constexpr float Z_BACKGROUND = -100;
    // Use this as the z value to draw on top of everything (e.g. a HUD or FPS counter)
    static constexpr float Z_HUD = 100;

    float x = 0;
    float y = 0;
    float z = 0;
    float width = 0;
    float height = 0;
    float windowScale = 1;

    GLfloat rotationX = 0;
    GLfloat rotationY = 0;
    GLfloat scale = 1;

    Texture* texture;

    Image() {};
    Image(Texture* texture_) { texture = texture_; };
    Image(Texture* texture_, float x_, float y_, float z_, float width_, float height_, float windowScale_, float scale_ = 1.0) {
        texture = texture_; x = x_; y = y_; z = z_; width = width_; height = height_, windowScale = windowScale_; scale = scale_;
    };
    
    void draw();

private:
    void _drawList();
};
