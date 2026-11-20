#!/bin/sh
set -e

if [ "$1" = "format" ]; then
    if ! command -v clang-format >/dev/null 2>&1; then
        echo "Ошибка: утилита clang-format не найдена."
        exit 1
    fi

    if [ ! -d "src" ]; then
        echo "Ошибка: директория src не найдена."
        exit 1
    fi

    echo "Форматирование исходного кода в папке src (без vendor)..."
    find src -path "*/vendor" -prune -o \
        \( -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.cc" -o -name "*.cxx" -o -name "*.c" -o -name "*.h" -o -name "*.hxx" \) \
        -exec clang-format -i --verbose -style=file -fallback-style=Google {} + \)
    echo "Форматирование завершено."
    exit 0
fi

if [ "$1" = "clean" ]; then
    echo "Очистка файлов сборки..."
    rm -rf CMakeCache.txt CMakeFiles Makefile *.project *.workspace
fi

cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug .
cmake --build .
