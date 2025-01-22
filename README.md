# Vandior

[![ci](https://github.com/Giuseppe-Bianc/Vandior/actions/workflows/ci.yml/badge.svg)](https://github.com/Giuseppe-Bianc/Vandior/actions/workflows/ci.yml)
[![codecov](https://codecov.io/gh/Giuseppe-Bianc/Vandior/graph/badge.svg?token=hLEkhzV7uE)](https://codecov.io/gh/Giuseppe-Bianc/Vandior)
[![CodeQL](https://github.com/Giuseppe-Bianc/Vandior/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/Giuseppe-Bianc/Vandior/actions/workflows/codeql-analysis.yml)
[![Lines of Code](https://sonarcloud.io/api/project_badges/measure?project=Giuseppe-Bianc_Vandior&metric=ncloc)](https://sonarcloud.io/summary/new_code?id=Giuseppe-Bianc_Vandior)

## About Vandior

Vandior is a modern transpiler, expertly built with the latest, C++20. It’s our aim to offer a flexible tool that caters
to a wide range of programming languages, assisting developers in handling and translating their code. While our goal is
ambitious, the project is in its development phase, and our team is working round the clock to improve its features.
Currently, Vandior only supports C++, but we’re well aware of the diverse programming landscape and are eager to broaden
our compatibility to include more languages soon.

## Requirements

To run this program smoothly, please ensure the following prerequisites are in place:

- Any C++ compiler is necessary for converting the program code into a runnable format on your system. There's a wide
  range of C++ compilers to choose from, pick one that suits your needs best.
- Python is also required, specifically for conducting compile-time evaluations. Python is a multifaceted language,
  popularly used for tasks like data analysis, web development, and machine learning, among other things.

## More Details

* [Dependency Setup](README_dependencies.md): This section provides a comprehensive guide to setting up the necessary
  dependencies for Vandior.It walks you through each step of the process, ensuring that you have everything you need to
  get started.
* [Building Details](README_building.md): Here, you can find detailed instructions on how to build Vandior. Whether
  you're a seasoned developer or a novice, these instructions will help you get Vandior up and running on your machine.
* [Docker](README_docker.md): This section provides information on how to use Vandior with Docker. If you're looking to
  containerize your Vandior applications, this guide will be invaluable.

## Usage

To get the most out of the project, you'll need to set a VNHOME environment variable. This variable should point to the
project's root path. This is important because it helps the compiler find the base header files it needs to run the
project correctly. If this isn't set, the compiler may run into errors or not work properly. So, setting the VNHOME
environment variable correctly is a key part of ensuring that the project runs smoothly.

```bash
vandior -i input_file.vn [-c] [-r] [-x] [-m]
```

Flags:

- --input (-i): the vn input source file;
- --compile (-c): compiles the resulting C++ file if no error occurs;
- --run (-r): Compiles and executes the resulting C++ file if no error occurs.
- --clean (-x): Clean before building.
- --cmake (-m): Create a CMakeLists.txt file.

[//]: # (Using -c and -r together is the same as using only -r.)
[//]: # ()
[//]: # ()
[//]: # (To manually compile the output C++ code:)
[//]: # ()
[//]: # ()
[//]: # (### Windows)
[//]: # ()
[//]: # ()
[//]: # (```powershell)
[//]: # ()
[//]: # (g++ -std=c++20 input_file.cpp -I %VNHOME%)
[//]: # ()
[//]: # (```)
[//]: # ()
[//]: # (### Unix)
[//]: # ()
[//]: # (```bash)
[//]: # (g++ -std=c++20 input_file.cpp -I $VNHOME)
[//]: # (```)