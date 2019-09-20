#!/bin/bash

mkdir -p /src/build/dc && cd /src/build/dc && cmake -DCMAKE_TOOLCHAIN_FILE=../../toolchains/Dreamcast.cmake ../../ && make -j4
