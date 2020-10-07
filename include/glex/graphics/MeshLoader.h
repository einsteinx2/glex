#pragma once
#include "glex/common/mesh.h"

#include <string>

class MeshLoader {
public:
    static MeshData* loadObjMesh(std::string path);
};