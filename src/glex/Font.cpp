#include "glex/Font.h"
#include "debug_log.h"
#include "fonts/arial_16pt.h"
#include "fonts/arial_28pt.h"
#include "fonts/arial_32pt.h"

#include <cstdio>
#include <cstdlib>

Font::~Font() {
    deleteTexture();
}

Font::Font(FontFace f, float scale_, float kerning_) {
    switch(f) {
    case arial_16:     _font = arial_16pt; break;
    case arial_28:     _font = arial_28pt; break;
    case arial_32:     _font = arial_32pt; break;
    default:
        printf("Unsupported font type\n");
        exit(EXIT_FAILURE);
    }

    scale = scale_;
    kerning = kerning_;
}

void Font::createTexture() {
    glGenTextures(1, &_textureId);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    #ifdef DREAMCAST
    // GLdc doesn't support GL_ALPHA textures, so convert to RGBA format
    uint32_t alphaSize = _font.tex_data.size();
    unsigned char rgbData[alphaSize*4];
    for(uint32_t i = 0; i < alphaSize; ++i) {
        rgbData[i*4] = rgbData[(i*4)+1] = rgbData[(i*4)+2] = 0xFF; // Set RGB values to max
        rgbData[(i*4)+3] = _font.tex_data[i];                      // Use existing alpha value
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _font.tex_width, _font.tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &rgbData[0]);
    #else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, _font.tex_width, _font.tex_height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, &_font.tex_data[0]);
    #endif
}

void Font::deleteTexture() {
    if (_textureId != 0) {
        glDeleteTextures(1, &_textureId);
        _textureId = 0;
    }
}

void Font::draw(float penX, float penY, std::string const& text) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);

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
        //         //printf("text[i]: %s text[i-1]: %s\n", &text[i], &text[i-1]);
        //         if (glyph->kerning[j].codepoint == text[i-1]) {
        //             k = glyph->kerning[j].kerning;
        //             //printf("text[i]: %d text[i+1]: %d k: %f\n", &text[i], &text[i-1], k);
        //             break;
        //         }
        //     }
        // }

        penX += kerning;
        float x = penX + glyph->offset_x;
        float y = penY + glyph->offset_y;
        float w  = glyph->width;
        float h  = glyph->height;
        
        glBegin(GL_TRIANGLES); {
            glColor4f(1.0, 1.0, 1.0, 1.0);
            glTexCoord2f(glyph->s0, glyph->t0 ); glVertex2f(x,     y  );
            glTexCoord2f(glyph->s0, glyph->t1 ); glVertex2f(x,     y - h);
            glTexCoord2f(glyph->s1, glyph->t1 ); glVertex2f(x + w, y - h);
            glTexCoord2f(glyph->s0, glyph->t0 ); glVertex2f(x,     y  );
            glTexCoord2f(glyph->s1, glyph->t1 ); glVertex2f(x + w, y - h);
            glTexCoord2f(glyph->s1, glyph->t0 ); glVertex2f(x + w, y  );
        } glEnd();
        
        penX += glyph->advance_x;
        penY += glyph->advance_y;
    }

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}
