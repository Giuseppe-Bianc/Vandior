#!/bin/bash
readonly VANDIOR_ROOT="$PWD"

readonly RUN_DIR="$VANDIOR_ROOT/build/src/Vandior/"
clear
cmake -S . -B ./build -Wno-dev -GNinja -DVandior_ENABLE_SANITIZER_ADDRESS=OFF -DVandior_WARNINGS_AS_ERRORS=ON -DVandior_ENABLE_CLANG_TIDY:BOOL=ON
cmake --build ./build -j 3
if [ -d "$RUN_DIR" ]; then
    cd  "$RUN_DIR"
    echo "Current working directory: $(pwd)"
    #valgrind --time-stamp=yes --leak-check=full --track-origins=yes --vgdb=no --gen-suppressions=all --num-callers=20 --error-limit=no --freelist-vol=1000000 --expensive-definedness-checks=yes --undef-value-errors=yes --keep-stacktraces=alloc --read-inline-info=yes --partial-loads-ok=yes --trace-children=yes --demangle=yes --show-leak-kinds=all ./vandior
    valgrind --tool=callgrind --time-stamp=yes --cache-sim=yes --branch-sim=yes --instr-atstart=yes --trace-children=yes --vgdb=no ./vandior
    echo "complete the valgrind work in $(pwd)"
    # Find the latest callgrind.out.<PID> file
    CALLGRIND_FILE=$(ls -t callgrind.out.* 2>/dev/null | head -n 1)

    if [ -z "$CALLGRIND_FILE" ]; then
        echo "No callgrind.out.<PID> file found!"
        exit 1
    else
        echo "Found Callgrind output file: $CALLGRIND_FILE"
    fi
else
    echo "Directory $RUN_DIR does not exist."
fi