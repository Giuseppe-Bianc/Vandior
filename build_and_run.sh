#!/bin/bash
clear && cmake -S . -B ./build -Wno-dev -GNinja -DVandior_WARNINGS_AS_ERRORS=ON -DVandior_ENABLE_CLANG_TIDY:BOOL=ON -DGLM_ENABLE_CXX_20:BOOL=ON -DGLM_ENABLE_SIMD_AVX2:BOOL=ON && cmake --build ./build -j 3
cd ./build/src/Vandior/ && ./vandior