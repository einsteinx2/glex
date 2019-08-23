#!/bin/bash

if [ "$#" -ne 1 ]; then
    printf "Usage: ./make.sh src_dir\n"
    printf "Build product will be placed into the 'out' folder\n\n"
    exit 1
fi

#cc `pkg-config --cflags glfw3` -o ./out/$1 glad_gl.c $1/*.c `pkg-config --static --libs glfw3`
#FILES=`ls $1/*.\(c\|cpp\)`
# files() {
#     ls $1/*.\(c\|cpp\)
# }
# files $1
#FILES=`find $1 -name "*.c" -exec echo -n "'{}' " \; -o -name "*.cpp" -exec echo -n '"{}" ' \;`
FILES=`find $1 -name "*.c" -o -name "*.cpp" | xargs echo`
clang++ -g -std=c++11 -stdlib=libc++ `pkg-config --cflags glfw3` -o ./out/$1 glad_gl.c $FILES `pkg-config --static --libs glfw3`

