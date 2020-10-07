#!/bin/bash

# Clean
rm -rf ./build/dc

# Build using max available threads
#export THREADS=$(getconf _NPROCESSORS_ONLN) &&
export THREADS=1 &&
# export CMAKE_BUILD_PARALLEL_LEVEL=$(getconf _NPROCESSORS_ONLN) &&
mkdir -p ./build/dc && cd ./build/dc &&
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../../cmake/toolchains/Dreamcast.cmake ../../ &&
make VERBOSE=ON -j$THREADS
# cmake --build . --clean-first -j 4
# cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../../cmake/toolchains/Dreamcast.cmake ../../ &&
# ninja

