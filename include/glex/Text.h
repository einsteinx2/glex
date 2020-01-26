#pragma once
#include "glex/common/gl.h"
#include "glex/common/font.h"
#include "Texture.h"

#include <string>

enum FontFace {
    arial_16,
    arial_28,
    arial_32
};

struct FontColor {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};
bool operator==(const FontColor& lhs, const FontColor& rhs);
#define FONT_COLOR_WHITE (FontColor){ .r = 255, .g = 255, .b = 255 }
#define FONT_COLOR_BLACK (FontColor){ .r = 0,   .g = 0,   .b = 0   }
#define FONT_COLOR_RED   (FontColor){ .r = 255, .g = 0,   .b = 0   }
#define FONT_COLOR_GREEN (FontColor){ .r = 0,   .g = 255, .b = 0   }
#define FONT_COLOR_BLUE  (FontColor){ .r = 0,   .g = 0,   .b = 255 }

class Text {
public:
    float x = 0;
    float y = 0;
    float z = 0;
    float windowScale = 1;

    GLfloat rotationX = 0;
    GLfloat rotationY = 0;
    float scale = 1;
    float kerning = 0; // TODO: Figure out sane default value

    std::string text;
    const FontColor& color = _color;
    const Texture& texture = _texture; // Texture object referencing the internal font texture

    Text(FontFace face, std::string text_, FontColor color_, float x_, float y_, float z_, float windowScale_, float scale_ = 1.0, float kerning_ = 0.0);
    ~Text();
    void createTexture();
    void deleteTexture();
    void draw();
private:
    FontColor _color;
    texture_font_t _font;
    Texture _texture;

    void _drawList();
};
