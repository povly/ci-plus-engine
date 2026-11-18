#!/bin/sh
set -e

if [ "$1" = "clean" ]; then
    echo "Очистка файлов сборки..."
    rm -rf CMakeCache.txt CMakeFiles Makefile *.project *.workspace
fi

cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug .
cmake --build .
