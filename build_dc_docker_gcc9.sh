#!/bin/bash

# Pinning specific docker image tag to ensure building always works
docker run -it --rm -v "$PWD:/src" einsteinx2/dcdev-kos-toolchain:gcc-9__v1.0.2 ./build_dc.sh
