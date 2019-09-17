#pragma once
#include <vector>

using namespace std;
struct MeshData {
    const int numVertices;
    const vector<float> vertices;
    const vector<float> textureCoordinates;
    const vector<float> normals;
};