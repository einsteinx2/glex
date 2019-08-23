#!/bin/bash

if [ "$#" -ne 1 ]; then
    printf "Usage: ./make.sh src_dir\n"
    printf "Build product will be placed into the 'out' folder\n\n"
    exit 1
fi

cc `pkg-config --cflags glfw3` -o ./out/$1 glad_gl.c $1/*.c `pkg-config --static --libs glfw3`
