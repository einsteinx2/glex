#include "glex/MeshLoader.h"
#include "debug_log.h"

#include <fstream>
#include <cstring>
#include <cstdlib>

MeshData* MeshLoader::loadObjMesh(std::string path) { 
    DEBUG_PRINTLN("Started loading mesh %s", path.c_str());
    MeshObjInfo info = _getObjInfo(path);
    DEBUG_PRINTLN("Loaded info");
    MeshData* data = _readObjFile(path, info);
    DEBUG_PRINTLN("Loaded data");
    return data;
}

MeshObjInfo MeshLoader::_getObjInfo(std::string path) {
    MeshObjInfo info = {};
    
    // Open OBJ file
    DEBUG_PRINTLN("obj path: %s", path.c_str());
    std::ifstream inOBJ(path);
    if (inOBJ && !inOBJ.good()) {
        DEBUG_PRINTLN("ERROR OPENING OBJ FILE");
        exit(1);
    }
    
    // Read OBJ file
    while (!inOBJ.eof()) {
        std::string line;
        getline(inOBJ, line);
        std::string type = line.substr(0,2);
         
        if (type.compare("v ") == 0)
            info.positions++;
        else if (type.compare("vt") == 0)
            info.texels++;
        else if (type.compare("vn") == 0)
            info.normals++;
        else if (type.compare("f ") == 0)
            info.faces++;
    }
    
    info.vertices = info.faces * 3;
    
    // Close OBJ file
    inOBJ.close();
    
    return info;
}

MeshData* MeshLoader::_readObjFile(std::string path, MeshObjInfo info) {
    float positions[info.positions][3];    // XYZ
    float texels[info.texels][2];          // UV
    float normals[info.normals][3];        // XYZ
    int faces[info.faces][9];              // PTN PTN PTN

    // Open OBJ file
    std::ifstream inOBJ;
    inOBJ.open(path);
    if (!inOBJ.good()) {
        DEBUG_PRINTLN("ERROR OPENING OBJ FILE");
        
        exit(1);
    }

    // Counters
    int p = 0;
    int t = 0;
    int n = 0;
    int f = 0;
    
    // Read OBJ file
    while (!inOBJ.eof()) {
        std::string line;
        getline(inOBJ, line);
        std::string type = line.substr(0,2);
        
        // Vertices
        if (type.compare("v ") == 0) {
            // Copy line for parsing
            char* l = new char[line.size() + 1];
            std::memcpy(l, line.c_str(), line.size() + 1);
            
            // Extract tokens
            std::strtok(l, " ");
            for (int i = 0; i < 3; i++) {
                positions[p][i] = std::atof(std::strtok(NULL, " "));
            }

            // Cleanup
            delete[] l;
            p++;
        }
        
        // Texture Coordinates
        else if (type.compare("vt") == 0) {
            // Copy line for parsing
            char* l = new char[line.size() + 1];
            std::memcpy(l, line.c_str(), line.size() + 1);
            
            // Extract tokens
            std::strtok(l, " ");
            for (int i = 0; i < 2; i++) {
                texels[t][i] = std::atof(std::strtok(NULL, " "));
            }

            // Cleanup
            delete[] l;
            t++;
        }
        
        // Normals
        else if (type.compare("vn") == 0) {
            char* l = new char[line.size() + 1];
            std::memcpy(l, line.c_str(), line.size() + 1);
            
            std::strtok(l, " ");
            for (int i = 0; i < 3; i++) {
                normals[n][i] = std::atof(std::strtok(NULL, " "));
            }

            // Cleanup
            delete[] l;
            n++;
        }

        // Faces
        else if (type.compare("f ") == 0) {
            char* l = new char[line.size() + 1];
            std::memcpy(l, line.c_str(), line.size() + 1);
            
            std::strtok(l, " ");
            for(int i=0; i < 9; i++) {
                faces[f][i] = std::atof(std::strtok(NULL, " /"));
            }

            delete[] l;
            f++;
        }
    }

    // Close OBJ file
    inOBJ.close();

    std::vector<float> finalVertices(info.vertices * 3);
    std::vector<float> finalTextCoords(info.vertices * 2);
    std::vector<float> finalNormals(info.vertices * 3);

    // Final Vertices
    int count = 0;
    for (int i = 0; i < info.faces; i++) {
        int vA = faces[i][0] - 1;
        int vB = faces[i][3] - 1;
        int vC = faces[i][6] - 1;
        
        finalVertices[count] = positions[vA][0]; finalVertices[count+1] = positions[vA][1]; finalVertices[count+2] = positions[vA][2];
        count += 3;
        finalVertices[count] = positions[vB][0]; finalVertices[count+1] = positions[vB][1]; finalVertices[count+2] = positions[vB][2];
        count += 3;
        finalVertices[count] = positions[vC][0]; finalVertices[count+1] = positions[vC][1]; finalVertices[count+2] = positions[vC][2];
        count += 3;        
    }

    // Final Tex Coords
    count = 0;
    for (int i = 0; i < info.faces; i++) {
        int vtA = faces[i][1] - 1;
        int vtB = faces[i][4] - 1;
        int vtC = faces[i][7] - 1;
        
        // Note that the Y coordinate is inverted as OpenGL requires that (alternatively you could vertically flip the image)
        finalTextCoords[count] = texels[vtA][0]; finalTextCoords[count+1] = (1.0 - texels[vtA][1]);
        count += 2;
        finalTextCoords[count] = texels[vtB][0]; finalTextCoords[count+1] = (1.0 - texels[vtB][1]);
        count += 2;
        finalTextCoords[count] = texels[vtC][0]; finalTextCoords[count+1] = (1.0 - texels[vtC][1]);
        count += 2;
    }

    // Final Normals
    count = 0;
    for (int i = 0; i < info.faces; i++) {
        int vnA = faces[i][2] - 1;
        int vnB = faces[i][5] - 1;
        int vnC = faces[i][8] - 1;
        
        finalNormals[count] = normals[vnA][0]; finalNormals[count+1] = normals[vnA][1]; finalNormals[count+2] = normals[vnA][2];
        count += 3;
        finalNormals[count] = normals[vnB][0]; finalNormals[count+1] = normals[vnB][1]; finalNormals[count+2] = normals[vnB][2];
        count += 3;
        finalNormals[count] = normals[vnC][0]; finalNormals[count+1] = normals[vnC][1]; finalNormals[count+2] = normals[vnC][2];
        count += 3;
    }

    MeshData* meshData = new MeshData();
    meshData->numVertices = info.vertices;
    meshData->vertices = finalVertices;
    meshData->textureCoordinates = finalTextCoords;
    meshData->normals = finalNormals ;
    return meshData;
}