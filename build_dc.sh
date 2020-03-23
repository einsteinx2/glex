#!/bin/bash

# Load KOS environment
. /opt/toolchains/dc/kos/environ.sh

# Build
mkdir -p /src/build/dc && cd /src/build/dc &&
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../../cmake/toolchains/Dreamcast.cmake ../../ &&
make -j8

