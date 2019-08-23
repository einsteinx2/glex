#include "Font.h"
#include "../common_gl.h"
#include "../debug_log.h"
#include "fonts/arial-16.h"

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

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

void Font::printAt(int penX, int penY, std::string const& text) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);

    size_t i, j;
    for(i = 0; i < text.length(); ++i) {
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
        int x = penX + glyph->offset_x;
        int y = penY + glyph->offset_y;
        int w  = glyph->width;
        int h  = glyph->height;
        
        glBegin(GL_TRIANGLES); {
            glColor4f(1.0, 1.0, 1.0, 1.0);
            glTexCoord2f(glyph->s0, glyph->t0 ); glVertex2i(x,   y  );
            glTexCoord2f(glyph->s0, glyph->t1 ); glVertex2i(x,   y-h);
            glTexCoord2f(glyph->s1, glyph->t1 ); glVertex2i(x+w, y-h);
            glTexCoord2f(glyph->s0, glyph->t0 ); glVertex2i(x,   y  );
            glTexCoord2f(glyph->s1, glyph->t1 ); glVertex2i(x+w, y-h);
            glTexCoord2f(glyph->s1, glyph->t0 ); glVertex2i(x+w, y  );
        } glEnd();
        
        penX += glyph->advance_x;
        penY += glyph->advance_y;
    }

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}