// This is a .h file for the model: cube.obj

// Positions: 8
// Texels: 14
// Normals: 6
// Faces: 12
// Vertices: 36

#pragma once

struct CubeMesh {
    const int numVertices;
    const float vertices[36][3];
    const float textureCoordinates[36][2];
    const float normals[36][3];
};

extern struct CubeMesh cubeMesh;
