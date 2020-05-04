#pragma once

#include <sys/stat.h>

#include <string>

namespace glex {
    // Check if a path exists
    static inline bool pathExists(const std::string& path) {
        struct stat buffer;   
        return stat(path.c_str(), &buffer) == 0; 
    }

    // Takes a path and modifies it depending on the target platform
    static inline std::string targetPlatformPath(const std::string& path) {
#ifdef DREAMCAST
        // Automatically use /pc if the /cd path doesn't exist to make
        // it easier for dcload users
        std::string platformPath = "/cd/" + path;
        if (!pathExists(platformPath)) {
            platformPath = "/pc/" + path;
        }
        return platformPath;
#endif
        return path;
    }
}