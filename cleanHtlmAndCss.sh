#!/bin/bash

readonly VANDIOR_ROOT="$PWD"

readonly COBERTURA_F="$VANDIOR_ROOT/out/cobertura.xml"

readonly COVERAGE_DIR="$VANDIOR_ROOT/out/coverage"
readonly BUID_DIR="$VANDIOR_ROOT/build"
if [ -d "$COVERAGE_DIR" ]; then
    cd  "$COVERAGE_DIR"
    echo "Cleaning up .html and .css files in $COVERAGE_DIR"

    
    # Remove all .html files if they exist
    if ls "$COVERAGE_DIR"/*.html 1> /dev/null 2>&1; then
        rm "$COVERAGE_DIR"/*.html
    else
        echo "No .html files found to delete."
    fi

    # Remove all .css files if they exist
    if ls "$COVERAGE_DIR"/*.css 1> /dev/null 2>&1; then
        rm "$COVERAGE_DIR"/*.css
    else
        echo "No .css files found to delete."
    fi


    # Remove cobertura.xml file if it exists
    if [ -f "$VANDIOR_ROOT/out/cobertura.xml" ]; then
        rm "$COBERTURA_F"
    else
        echo "No $COBERTURA_F file found to delete."
    fi


    echo "Cleanup complete."
else
    echo "Directory $COVERAGE_DIR does not exist."
fi