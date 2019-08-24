#pragma once
#include "../../common_gl.h"
#include "../meshes/common_mesh.hpp"
#include "Texture.hpp"

class Mesh {
    public:
        GLfloat rotationX;
        GLfloat rotationY;
        GLfloat rotationZ;
        GLfloat scale;
        Mesh(MeshData* meshData, Texture* texture, GLfloat scale = 1.0);
        void draw();
    private:
        GLint _list;
        MeshData* _meshData;
        Texture* _texture;
        GLfloat _scale;
};