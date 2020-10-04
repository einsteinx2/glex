#include "glex/Cube.h"
#include "glex/common/log.h"

Cube::Cube() {
}

Cube::~Cube() {
    DEBUG_PRINTLN("Destroy Cube!");
}

Cube::Cube(Texture* texture) {
    setTexture(texture);
}

Texture* Cube::getTexture() {
    return _texture;
}

void Cube::setTexture(Texture* texture) {
    _texture = texture;
}

void Cube::_drawList() {
    #ifdef GLFW
    // Enable texture
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, _texture->id);
    #endif
    
    // cube ///////////////////////////////////////////////////////////////////////
    //    v7----- v4
    //   /|      /|
    //  v3------v0|
    //  | |     | |
    //  | |v6---|-|v5
    //  |/      |/
    //  v2------v1

    GLfloat v0[3] = {  1.0f,  1.0f,  1.0f };
    GLfloat v1[3] = {  1.0f, -1.0f,  1.0f };
    GLfloat v2[3] = { -1.0f, -1.0f,  1.0f };
    GLfloat v3[3] = { -1.0f,  1.0f,  1.0f };
    GLfloat v4[3] = {  1.0f,  1.0f, -1.0f };
    GLfloat v5[3] = {  1.0f, -1.0f, -1.0f };
    GLfloat v6[3] = { -1.0f, -1.0f, -1.0f };
    GLfloat v7[3] = { -1.0f,  1.0f, -1.0f };

    glBegin(GL_QUADS);  // draw a cube with 6 quads

    #ifdef DREAMCAST
    glColor3f(1.0, 0.0, 0.0); 
    glVertex3fv(v0);    // front face
    glVertex3fv(v3);
    glVertex3fv(v2);
    glVertex3fv(v1);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3fv(v4);    // top face
    glVertex3fv(v7);
    glVertex3fv(v3);
    glVertex3fv(v0);

    glColor3f(0.0, 0.0, 1.0);
    glVertex3fv(v3);    // left face
    glVertex3fv(v7);
    glVertex3fv(v6);
    glVertex3fv(v2);

    glColor3f(1.0, 0.0, 0.0); 
    glVertex3fv(v4);    // back face
    glVertex3fv(v7);
    glVertex3fv(v6);
    glVertex3fv(v5);

    glColor3f(0.0, 0.0, 1.0);
    glVertex3fv(v4);    // right face
    glVertex3fv(v0);
    glVertex3fv(v1);
    glVertex3fv(v5);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3fv(v5);    // bottom face
    glVertex3fv(v6);
    glVertex3fv(v2);
    glVertex3fv(v1);

    glEnd();
    #else
    glNormal3f(0, 0, 1); 

    glColor3f(1.0, 1.0, 1.0); 
    glTexCoord2f(0, 0);
    glVertex3fv(v0);    // front face
    glTexCoord2f(1, 0);
    glVertex3fv(v3);
    glTexCoord2f(1, 1);
    glVertex3fv(v2);
    glTexCoord2f(0, 1);
    glVertex3fv(v1);

    glTexCoord2f(0, 0);
    glVertex3fv(v4);    // top face
    glTexCoord2f(1, 0);
    glVertex3fv(v7);
    glTexCoord2f(1, 1);
    glVertex3fv(v3);
    glTexCoord2f(0, 1);
    glVertex3fv(v0);

    glTexCoord2f(0, 0);
    glVertex3fv(v3);    // left face
    glTexCoord2f(1, 0);
    glVertex3fv(v7);
    glTexCoord2f(1, 1);
    glVertex3fv(v6);
    glTexCoord2f(0, 1);
    glVertex3fv(v2);

    glTexCoord2f(0, 0);
    glVertex3fv(v4);    // back face
    glTexCoord2f(1, 0);
    glVertex3fv(v7);
    glTexCoord2f(1, 1);
    glVertex3fv(v6);
    glTexCoord2f(0, 1);
    glVertex3fv(v5);

    glTexCoord2f(0, 0);
    glVertex3fv(v4);    // right face
    glTexCoord2f(1, 0);
    glVertex3fv(v0);
    glTexCoord2f(1, 1);
    glVertex3fv(v1);
    glTexCoord2f(0, 1);
    glVertex3fv(v5);

    glTexCoord2f(0, 0);
    glVertex3fv(v5);    // bottom face
    glTexCoord2f(1, 0);
    glVertex3fv(v1);
    glTexCoord2f(1, 1);
    glVertex3fv(v2);
    glTexCoord2f(0, 1);
    glVertex3fv(v6);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    #endif
}

void Cube::draw() {
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glPushMatrix();
    glRotatef(_anglez, 0.0f, 0.0f, 1.0f);
    glRotatef(_angley, 0.0f, 1.0f, 0.0f);
    glRotatef(_anglex, 1.0f, 0.0f, 0.0f);
    
    glScalef(3.0, 3.0, 3.0);
    
    _drawList();
    glPopMatrix();

    _anglez += 0.5;
    _angley -= 0.5;
    _anglex += 0.5;
}
