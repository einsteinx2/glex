#include "Font.h"
#include "../common_gl.h"
#include "../debug_log.h"
#include "fonts/arial-32.h"

#include <stdlib.h>
#include <stdio.h>

void Font::createTexture() {
    GLuint texid;
    glGenTextures(1, &texid);
    glBindTexture(GL_TEXTURE_2D, texid);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, font.tex_width, font.tex_height,
                 0, GL_ALPHA, GL_UNSIGNED_BYTE, font.tex_data);
    glBindTexture(GL_TEXTURE_2D, texid);
}

void Font::printAt(float penX, float penY, float kerning, std::string const& text) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);

    size_t i, j;
    for(i = 0; i < text.length(); ++i) {
        // Find the glyph
        texture_glyph_t* glyph = 0;
        for(j = 0; j < font.glyphs_count; ++j) {
            if (font.glyphs[j].charcode == text[i]) {
                glyph = &font.glyphs[j];
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