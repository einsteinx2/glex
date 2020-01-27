#!/bin/bash

mkdir -p ./build/mac && cd ./build/mac && 
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../../cmake/toolchains/MacOS.cmake ../../ && 
make -j4

