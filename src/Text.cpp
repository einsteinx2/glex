#include "glex/Text.h"
#include "glex/common/log.h"
#include "glex/fonts/arial_16pt.h"
#include "glex/fonts/arial_28pt.h"
#include "glex/fonts/arial_32pt.h"

#include <cstdlib>

// TODO: Fix font rendering (it has weird smaller ghost characters inside the normal characters)

bool operator==(const FontColor& lhs, const FontColor& rhs) {
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

Text::~Text() {
    deleteTexture();
}

Text::Text(FontFace face_, std::string text_, FontColor color_, float x_, float y_, float z_, float windowScale_, float scale_, float kerning_) {
    switch(face_) {
    case arial_16:     _font = arial_16pt; break;
    case arial_28:     _font = arial_28pt; break;
    case arial_32:     _font = arial_32pt; break;
    default:
        DEBUG_PRINTLN("Unsupported font type");
        exit(EXIT_FAILURE);
    }

    _color = color_;
    text = text_;
    x = x_;
    y = y_;
    z = z_;
    windowScale = windowScale_;
    scale = scale_;
    kerning = kerning_;
}

void Text::createTexture() {
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

    _texture.loadExisting(_font.tex_width, _font.tex_height, textureId);
}

void Text::deleteTexture() {
    if (_texture.id != 0) {
        glDeleteTextures(1, &_texture.id);
        _texture.id = 0;
    }
}

void Text::draw() {
    // Set OpenGL draw settings
    glDisable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Not sure if I need these or not
    //glAlphaFunc(GL_GREATER, 0.f);
    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glPushMatrix();

    // Perform scaling and rotation
    //DEBUG_PRINTLN("Font scale: %f  windowScale: %f  xScale: %f  yScale: %f", scale, windowScale, scale * windowScale, scale * windowScale);
    //glScalef(scale * windowScale, scale * windowScale, 1.0);
    //glScalef(scale, scale, 1.0);
    glScalef(scale, scale, 1.0);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);

    _drawList();

    glDisable(GL_BLEND);
}

void Text::_drawList() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _texture.id);

    size_t i, j;
    float ix = x, iy = y;
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

        // Calculate the size and location based on the current character's glyph
        float ox = ix + glyph->offset_x + kerning;
        float oy = iy + glyph->offset_y;
        float w  = glyph->width;
        float h  = glyph->height;
        
        // Draw the letter
        glBegin(GL_TRIANGLES);
        glColor4f(1.0, 1.0, 1.0, 1.0);
        glTexCoord2f(glyph->s0, glyph->t0); glVertex3f(ox,     oy    , z);
        glTexCoord2f(glyph->s0, glyph->t1); glVertex3f(ox,     oy - h, z);
        glTexCoord2f(glyph->s1, glyph->t1); glVertex3f(ox + w, oy - h, z);
        glTexCoord2f(glyph->s0, glyph->t0); glVertex3f(ox,     oy    , z);
        glTexCoord2f(glyph->s1, glyph->t1); glVertex3f(ox + w, oy - h, z);
        glTexCoord2f(glyph->s1, glyph->t0); glVertex3f(ox + w, oy    , z);
        glEnd();
        
        // Advance to the next letter's position
        ix += glyph->advance_x;
        iy += glyph->advance_y;
    }

    glDisable(GL_TEXTURE_2D);
}