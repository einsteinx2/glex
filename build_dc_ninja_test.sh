#!/bin/bash

# Clean
rm -rf ./build/dc

# Build using max available threads
mkdir -p ./build/dc && cd ./build/dc &&
cmake --build . --clean-first -j 4
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../../cmake/toolchains/Dreamcast.cmake ../../ &&
# ninja NSound && ninja GLdc && ninja
ninja GLdc && ninja PlaygroundExample