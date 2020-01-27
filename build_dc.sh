#!/bin/bash

mkdir -p /src/build/dc && cd /src/build/dc &&
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../../cmake/toolchains/Dreamcast.cmake ../../ &&
make -j4

