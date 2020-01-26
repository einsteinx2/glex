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

    GLfloat rotationX = 0.0;
    GLfloat rotationY = 0.0;
    GLfloat scale = 1.0;

    int width();
    int height();

    Image(Texture* texture, GLfloat scale = 1.0);
    void draw(float x, float y, float z, float width, float height, float windowScale = 1.0);

private:
    Texture* _texture;
    GLfloat _scale;

    void _drawList(float x, float y, float z, float w, float h, float ws);
};
