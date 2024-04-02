# Vandior

[![ci](https://github.com/Giuseppe-Bianc/Vandior/actions/workflows/ci.yml/badge.svg)](https://github.com/Giuseppe-Bianc/Vandior/actions/workflows/ci.yml)
[![codecov](https://codecov.io/gh/Giuseppe-Bianc/Vandior/graph/badge.svg?token=hLEkhzV7uE)](https://codecov.io/gh/Giuseppe-Bianc/Vandior)
[![CodeQL](https://github.com/Giuseppe-Bianc/Vandior/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/Giuseppe-Bianc/Vandior/actions/workflows/codeql-analysis.yml)
[![Lines of Code](https://sonarcloud.io/api/project_badges/measure?project=Giuseppe-Bianc_Vandior&metric=ncloc)](https://sonarcloud.io/summary/new_code?id=Giuseppe-Bianc_Vandior)

## About Vandior

A modern c++20 transpiler.

## Requirements

- Any c++ compiler;
- Python (used to compile time evaluations).

## More Details

* [Dependency Setup](README_dependencies.md)
* [Building Details](README_building.md)
* [Docker](README_docker.md)

## Usage

Define a VNHOME environment variable containing the path to the project root (This is necessary to allow the compiler find the base header files).
```
vandior -i input_file.vn [-c] [-r]
```
Flags:
- --input (-i): the vn input source file;
- --compile (-c): compiles the resulting C++ file if no error occurs;
- --run (-r): Compiles and executes the resulting C++ file if no error occurs.

Using -c and -r together is the same as using only -r.

To manually compile the output C++ code:

### Windows

```
g++ -std=c++20 input_file.cpp -I %VNHOME%
```

### Unix

```
g++ -std=c++20 input_file.cpp -I $VNHOME
```