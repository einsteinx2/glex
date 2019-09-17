#!/bin/bash
#cd /src
#rm -rf /src/CMakeCache.txt /src/CMakeFiles /src/build/dc && cmake -DCMAKE_TOOLCHAIN_FILE=./toolchains/Dreamcast.cmake . && make -j4

#rm -rf /src/build/dc && \
#mkdir -p /src/build/dc && cd /src/build/dc && cmake -DCMAKE_TOOLCHAIN_FILE=../../toolchains/Dreamcast.cmake ../../ && \
#cd /src/deps/dc/GLdc && make build -j4 && cd /src/build/dc && make -j4

cd /src/deps/dc/GLdc && make clean && make build && cd /src && \
mkdir -p /src/build/dc && cd /src/build/dc && cmake -DCMAKE_TOOLCHAIN_FILE=../../toolchains/Dreamcast.cmake ../../ && make -j4 && /src