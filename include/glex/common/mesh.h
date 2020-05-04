#pragma once
#include <vector>
#include <cstddef>

struct MeshData {
    size_t numVertices = 0;
    std::vector<float> vertices;
    std::vector<float> textureCoordinates;
    std::vector<float> normals;
};