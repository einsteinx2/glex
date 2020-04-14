#!/bin/bash

docker run -it --rm -v "$PWD:/src" einsteinx2/dcdev-kos-toolchain:gcc-4 ./build_dc.sh
