// This is a .h file for the model: rockTriangles.obj

// Positions: 5544
// Texels: 6648
// Normals: 21245
// Faces: 10968
// Vertices: 32904

#pragma once

struct RockTrianglesMesh {
    const int numVertices;
    const float vertices[32904][3];
    const float textureCoordinates[32904][2];
    const float normals[32904][3];
};

extern struct RockTrianglesMesh rockTrianglesMesh;
