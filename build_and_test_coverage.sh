#!/bin/bash
clear && cmake -S . -B ./build -Wno-dev -GNinja -DVandior_WARNINGS_AS_ERRORS=ON -DVandior_ENABLE_CLANG_TIDY:BOOL=ON  -DVandior_ENABLE_COVERAGE:BOOL=ON && cmake --build ./build -j 3
cd ./build && ctest -C Debug && cd ../ && gcovr -j 3  --config gcovr.cfg --gcov-executable 'gcov' --exclude-unreachable-branches --exclude-noncode-lines
#gcovr -j 3 --verbose --delete --root ../  --gcov-ignore-errors=all --print-summary --xml-pretty --xml out/cobertura.xml  --html-details out/coverage/index.html --gcov-executable 'gcov'
