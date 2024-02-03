#!/bin/bash
clear && cmake -S . -B ./build -Wno-dev -GNinja -DVandior_WARNINGS_AS_ERRORS=off  && cmake --build ./build
cd ./build/src/Vandior/ && ./vandior