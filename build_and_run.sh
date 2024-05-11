#!/bin/bash
clear && cmake -S . -B ./build -Wno-dev -GNinja -DVandior_WARNINGS_AS_ERRORS=off -DVandior_ENABLE_CLANG_TIDY:BOOL=OFF && cmake --build ./build -j 3
cd ./build/src/Vandior/ && ./vandior