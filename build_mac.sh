#!/bin/bash
#mkdir -p ./build/mac && cd ./build/mac && cmake ../../ && make -j4
mkdir -p ./build/mac && cd ./build/mac && cmake -DCMAKE_BUILD_TYPE=Debug ../../ && make -j4