#!/bin/bash
clear && cmake -S . -B ./build && cmake --build ./build
cd ./build/src/Vandior/ && ./vandior