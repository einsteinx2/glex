#pragma once
#include "../common_gl.h"
#include "../fonts/common_font.h"
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

class Font {
public:
    float scale;
    float kerning;
    const FontColor& color = _color;
    const Texture& texture = _texture;

    Font(FontFace face, FontColor color = FONT_COLOR_WHITE, float scale_ = 1.0, float kerning_ = 0.0);
    ~Font();
    void createTexture();
    void deleteTexture();
    //Texture texture(); // Returns a Texture object referencing the internal font texture
    void draw(float penX, float penY, std::string const& text, float windowScale = 1.0);
private:
    FontColor _color;
    texture_font_t _font;
    Texture _texture;

    void _drawList(float penX, float penY, std::string const& text, float ws);
};
