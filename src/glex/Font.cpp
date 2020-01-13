#include "glex/Font.h"
#include "debug_log.h"
#include "fonts/arial_16pt.h"
#include "fonts/arial_28pt.h"
#include "fonts/arial_32pt.h"

bool operator==(const FontColor& lhs, const FontColor& rhs) {
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

Font::~Font() {
    deleteTexture();
}

Font::Font(FontFace f, FontColor color_, float scale_, float kerning_) {
    switch(f) {
    case arial_16:     _font = arial_16pt; break;
    case arial_28:     _font = arial_28pt; break;
    case arial_32:     _font = arial_32pt; break;
    default:
        DEBUG_PRINTLN("Unsupported font type");
        //exit(EXIT_FAILURE);
    }

    scale = scale_;
    kerning = kerning_;
    _color = color_;
}

void Font::createTexture() {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Optimization for white text
    if (_color == FONT_COLOR_WHITE) {
        // Use 8bit alpha only texture to save memory
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, _font.tex_width, _font.tex_height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, &_font.tex_data[0]);
    } else {
        // Convert the texture data to 32bit RGBA
        size_t currentSize = _font.tex_data.size();
        size_t newSize = _font.tex_data.size() * 4;
        uint8_t* rgbData = new uint8_t[newSize];
        for (size_t i = 0; i < currentSize; i+=1) {
            rgbData[(i*4)]   = _color.r;
            rgbData[(i*4)+1] = _color.g;
            rgbData[(i*4)+2] = _color.b;
            rgbData[(i*4)+3] = _font.tex_data[i];
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _font.tex_width, _font.tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbData);
    }

    _texture.loadExistingTexture(_font.tex_width, _font.tex_height, textureId);
}

void Font::deleteTexture() {
    if (_texture.id != 0) {
        glDeleteTextures(1, &_texture.id);
        _texture.id = 0;
    }
}

void Font::draw(float penX, float penY, std::string const& text, float windowScale) {
    // Set OpenGL draw settings
    // glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glAlphaFunc(GL_GREATER, 0.f);
    // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glPushMatrix();

    // Perform scaling
    glScalef(scale * windowScale, scale * windowScale, 1.0);

    _drawList(penX * (1 / windowScale), penY * (1 / windowScale), text, windowScale);

    glDisable(GL_BLEND);
}

void Font::_drawList(float penX, float penY, std::string const& text, float ws) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _texture.id);

    size_t i, j;
    for(i = 0; i < text.length(); ++i) {
        // Find the glyph
        texture_glyph_t* glyph = 0;
        for(j = 0; j < _font.glyphs_count; ++j) {
            if (_font.glyphs[j].charcode == text[i]) {
                glyph = &_font.glyphs[j];
                break;
            }
        }
        if(!glyph) {
            continue;
        }

        // // Try to dynamically add kerning (NOT WORKING)
        //float k = 0;
        // if (i > 0) {
        //     for (j = 0; j < glyph->kerning_count; ++j) {
        //         //DEBUG_PRINTLN("text[i]: %s text[i-1]: %s", &text[i], &text[i-1]);
        //         if (glyph->kerning[j].codepoint == text[i-1]) {
        //             k = glyph->kerning[j].kerning;
        //             //DEBUG_PRINTLN("text[i]: %d text[i+1]: %d k: %f", &text[i], &text[i-1], k);
        //             break;
        //         }
        //     }
        // }

        penX += kerning;
        float x = penX + glyph->offset_x;
        float y = penY + glyph->offset_y;
        float w  = glyph->width;
        float h  = glyph->height;
        
        glBegin(GL_TRIANGLES);
        glColor4f(1.0, 1.0, 1.0, 1.0);
        ws = 1.0;
        // glTexCoord2f(glyph->s0, glyph->t0); glVertex2f(ws * x,     ws * y  );
        // glTexCoord2f(glyph->s0, glyph->t1); glVertex2f(ws * x,     ws * y - h);
        // glTexCoord2f(glyph->s1, glyph->t1); glVertex2f(ws * x + w, ws * y - h);
        // glTexCoord2f(glyph->s0, glyph->t0); glVertex2f(ws * x,     ws * y  );
        // glTexCoord2f(glyph->s1, glyph->t1); glVertex2f(ws * x + w, ws * y - h);
        // glTexCoord2f(glyph->s1, glyph->t0); glVertex2f(ws * x + w, ws * y  );
        glTexCoord2f(glyph->s0, glyph->t0); glVertex2f(x,     y  );
        glTexCoord2f(glyph->s0, glyph->t1); glVertex2f(x,     y - h);
        glTexCoord2f(glyph->s1, glyph->t1); glVertex2f(x + w, y - h);
        glTexCoord2f(glyph->s0, glyph->t0); glVertex2f(x,     y  );
        glTexCoord2f(glyph->s1, glyph->t1); glVertex2f(x + w, y - h);
        glTexCoord2f(glyph->s1, glyph->t0); glVertex2f(x + w, y  );
        glEnd();
        
        penX += glyph->advance_x;
        penY += glyph->advance_y;
    }

    glDisable(GL_TEXTURE_2D);
}