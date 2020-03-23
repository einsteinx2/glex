#!/bin/bash

docker run -it --rm -v "$PWD:/src" einsteinx2/dcdev-kos bash -c /src/build_dc.sh
