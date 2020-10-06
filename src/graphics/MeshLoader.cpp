#include "glex/graphics/MeshLoader.h"
#include "glex/common/log.h"
#include "glex/common/path.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cpp file
#include "objl/tiny_obj_loader.h"

#include <fstream>
#include <cstring>
#include <cstdlib>

MeshData* MeshLoader::loadObjMesh(std::string path) { 
    std::string platformPath = glex::targetPlatformPath(path);
    DEBUG_PRINTLN("Started loading mesh %s", platformPath.c_str());

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, platformPath.c_str());

    if (!warn.empty()) {
        ERROR_PRINTLN("Warning while loading mesh: %s", warn.c_str());
    }
    if (!err.empty()) {
        ERROR_PRINTLN("Error while loading mesh: %s", err.c_str());
    }
    if (!success) {
        return NULL;
    }

    // TODO: Load objs with more than one shape
    // TODO: Load materials
    // TODO: Switch to rendering the tiny_obj_loader format directly to save processing and memory
    MeshData* meshData = new MeshData();
    meshData->numVertices = shapes[0].mesh.indices.size();
    std::vector<float> finalVertices((size_t)meshData->numVertices * 3);
    std::vector<float> finalTextCoords((size_t)meshData->numVertices * 2);
    std::vector<float> finalNormals((size_t)meshData->numVertices * 3);
    int vertCount = 0;
    int textCoordCount = 0;
    int normalCount = 0;
    size_t index_offset = 0;
    for (size_t f = 0; f < shapes[0].mesh.num_face_vertices.size(); f++) {
        size_t fv = shapes[0].mesh.num_face_vertices[f];

        // Loop over vertices in the face.
        for (size_t v = 0; v < fv; v++) {

            // access to vertex
            tinyobj::index_t idx = shapes[0].mesh.indices[index_offset + v];
            // DEBUG_PRINTLN("idx - vertex %d  normal: %d  texCoord: %d", idx.vertex_index, idx.normal_index, idx.texcoord_index);
            tinyobj::real_t vx = attrib.vertices[3*(size_t)idx.vertex_index+0];
            tinyobj::real_t vy = attrib.vertices[3*(size_t)idx.vertex_index+1];
            tinyobj::real_t vz = attrib.vertices[3*(size_t)idx.vertex_index+2];
            finalVertices[vertCount] = vx; finalVertices[(size_t)vertCount+1] = vy; finalVertices[(size_t)vertCount+2] = vz;
            vertCount += 3;
            tinyobj::real_t nx = attrib.normals[3*(size_t)idx.normal_index+0];
            tinyobj::real_t ny = attrib.normals[3*(size_t)idx.normal_index+1];
            tinyobj::real_t nz = attrib.normals[3*(size_t)idx.normal_index+2];
            finalNormals[normalCount] = nx; finalNormals[(size_t)normalCount+1] = ny; finalNormals[(size_t)normalCount+2] = nz;
            normalCount += 3;
            tinyobj::real_t tx = attrib.texcoords[2*(size_t)idx.texcoord_index+0];
            tinyobj::real_t ty = attrib.texcoords[2*(size_t)idx.texcoord_index+1];
            finalTextCoords[textCoordCount] = tx; finalTextCoords[(size_t)textCoordCount+1] = ty;
            textCoordCount += 2;
            // Optional: vertex colors
            // tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
            // tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
            // tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
        }
        index_offset += fv;
    }
    meshData->vertices = finalVertices;
    meshData->textureCoordinates = finalTextCoords;
    meshData->normals = finalNormals;
    return meshData;
}