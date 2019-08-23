/* ============================================================================
 * Freetype GL - A C OpenGL Freetype engine
 * Platform:    Any
 * WWW:         http://code.google.com/p/freetype-gl/
 * ----------------------------------------------------------------------------
 * Copyright 2011,2012 Nicolas P. Rougier. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY NICOLAS P. ROUGIER ''AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL NICOLAS P. ROUGIER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of Nicolas P. Rougier.
 * ============================================================================
 */
#include "../common_gl.h"
#include "arial-16.h"

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>


void print_at( int pen_x, int pen_y, wchar_t *text )
{
    size_t i, j;
    for( i=0; i<wcslen(text); ++i)
    {
        texture_glyph_t *glyph = 0;
        for( j=0; j<font.glyphs_count; ++j)
        {
            if( font.glyphs[j].charcode == text[i] )
            {
                glyph = &font.glyphs[j];
                break;
            }
        }
        if( !glyph )
        {
            continue;
        }
        int x = pen_x + glyph->offset_x;
        int y = pen_y + glyph->offset_y;
        int w  = glyph->width;
        int h  = glyph->height;
        //printf("x: %d y: %d w: %d h: %d\n", x, y, w, h);
        glBegin( GL_TRIANGLES );
        {
            glColor4f(0.0, 0.0, 0.0, 1.0);
            glTexCoord2f( glyph->s0, glyph->t0 ); glVertex2i( x,   y   );
            glTexCoord2f( glyph->s0, glyph->t1 ); glVertex2i( x,   y-h );
            glTexCoord2f( glyph->s1, glyph->t1 ); glVertex2i( x+w, y-h );
            glTexCoord2f( glyph->s0, glyph->t0 ); glVertex2i( x,   y   );
            glTexCoord2f( glyph->s1, glyph->t1 ); glVertex2i( x+w, y-h );
            glTexCoord2f( glyph->s1, glyph->t0 ); glVertex2i( x+w, y   );
        }
        glEnd();
        pen_x += glyph->advance_x;
        pen_y += glyph->advance_y;

    }
}

/* exit upon ESC */
void key( GLFWwindow* window, int k, int s, int action, int mods )
{
  if( action != GLFW_PRESS ) return;

  switch (k) {
  case GLFW_KEY_ESCAPE:
    glfwSetWindowShouldClose(window, GLFW_TRUE);
    break;
  default:
    return;
  }
}

/* new window size */
void reshape( GLFWwindow* window, int width, int height )
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

int main( int argc, char **argv )
{
    GLFWwindow* window;
    int width, height;

    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit( EXIT_FAILURE );
    }

    glfwWindowHint(GLFW_DEPTH_BITS, 16);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    window = glfwCreateWindow( 640, 480, "Freetype OpenGL", NULL, NULL );
    if (!window)
    {
        fprintf( stderr, "Failed to open GLFW window\n" );
        glfwTerminate();
        exit( EXIT_FAILURE );
    }

    // Set callback functions
    glfwSetFramebufferSizeCallback(window, reshape);
    glfwSetKeyCallback(window, key);

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval( 1 );

    glfwGetFramebufferSize(window, &width, &height);
    reshape(window, width, height);

    GLuint texid;
    glGenTextures( 1, &texid );
    glBindTexture( GL_TEXTURE_2D, texid );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_ALPHA, font.tex_width, font.tex_height,
                  0, GL_ALPHA, GL_UNSIGNED_BYTE, font.tex_data );
    glBindTexture( GL_TEXTURE_2D, texid );

    glClearColor( 1.0, 1.0, 1.0, 1.0 );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_TEXTURE_2D );

    // Main loop
    while( !glfwWindowShouldClose(window) )
    {
        // Display the text
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        print_at(20, 20, L"Hello World !");

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();

    // Exit program
    exit( EXIT_SUCCESS );
}
