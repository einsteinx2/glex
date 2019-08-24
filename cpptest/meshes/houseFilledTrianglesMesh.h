// This is a .h file for the model: houseFilledTriangles.obj

// Positions: 337
// Texels: 566
// Normals: 222
// Faces: 616
// Vertices: 1848

#pragma once

struct HouseFilledTrianglesMesh {
    const int numVertices;
    const float vertices[1848][3];
    const float textureCoordinates[1848][2];
    const float normals[1848][3];
};

extern struct HouseFilledTrianglesMesh houseFilledTrianglesMesh;
