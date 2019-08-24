#include "../common_gl.h"
#include "fonts/common-font.hpp"

#include <string>

enum FontFace {
    arial_16,
    arial_28,
    arial_32,
    bitWonder_28
};

class Font {
    public:
        ~Font();
        float scale = 1.0;
        float kerning = 0.0;
        Font(FontFace face, float scale_ = 1.0, float kerning_ = 0.0);
        void createTexture();
        void deleteTexture();
        void draw(float penX, float penY, std::string const& text);
    private:
        texture_font_t _font;
        GLuint _textureId;
};