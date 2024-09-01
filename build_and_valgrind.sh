#!/bin/bash
clear && cmake -S . -B ./build -Wno-dev -GNinja -DVandior_ENABLE_SANITIZER_ADDRESS=OFF -DVandior_WARNINGS_AS_ERRORS=ON -DVandior_ENABLE_CLANG_TIDY:BOOL=ON && cmake --build ./build -j 3
cd ./build/src/Vandior/ &&  valgrind --time-stamp=yes --leak-check=full ./vandior