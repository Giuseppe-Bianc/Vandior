#!/bin/bash
readonly VANDIOR_ROOT="$PWD"

readonly RUN_DIR="$VANDIOR_ROOT/build/src/Vandior/"
clear
cmake -S . -B ./build -Wno-dev -GNinja -DVandior_ENABLE_SANITIZER_ADDRESS=OFF -DVandior_WARNINGS_AS_ERRORS=ON -DVandior_ENABLE_CLANG_TIDY:BOOL=ON
cmake --build ./build -j 3
if [ -d "$RUN_DIR" ]; then
    cd  "$RUN_DIR"
    echo "Current working directory: $(pwd)"
    valgrind --time-stamp=yes --leak-check=full ./vandior
    echo "complete."
else
    echo "Directory $RUN_DIR does not exist."
fi