#!/bin/bash
#apt install -y cmake genisoimage
#cd /opt/toolchains/dc/kos/utils && mkdir makeip && cd makeip && curl http://www.boob.co.uk/files/makeip.tar.gz > makeip.tar.gz && tar -xf makeip.tar.gz && rm makeip.tar.gz && gcc makeip.c -o makeip
#cd /opt/toolchains/dc/kos/utils && git clone https://github.com/Kazade/img4dc.git && cd img4dc && cmake . && make BUMP=9
cd /src
rm -rf /src/CMakeCache.txt /src/CMakeFiles /src/build/dc && cmake -DCMAKE_TOOLCHAIN_FILE=./toolchains/Dreamcast.cmake . && make -j4
