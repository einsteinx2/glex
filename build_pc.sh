#!/bin/bash

# Clean
rm -rf ./build/pc

# Build using max available threads
export THREADS=$(getconf _NPROCESSORS_ONLN) &&
mkdir -p ./build/pc && cd ./build/pc && 
cmake -DCMAKE_BUILD_TYPE=Debug ../../ && 
make VERBOSE=ON -j$THREADS

