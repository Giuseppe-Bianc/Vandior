# README_building

## Build Instructions

A full build has different steps:

1) Specifying the compiler using environment variables
2) Configuring the project
3) Building the project

If you need to tweak the source code in future builds—maybe to add new features, fix bugs, or just enhance its
functionality, you don't have to go through every single step again. You can just concentrate on repeating the final
step of the build process. This streamlined method lets you smoothly handle and apply your changes without causing a
major upset in the overall workflow.

### (1) Specify the compiler using environment variables

If the user doesn't provide specific settings, the system defaults to its standard compiler. This kicks in when
environment variables like `CC` and `CXX` are left undefined.

Software build management tool, CMake, relies on these variables, `CC` and `CXX`, to choose the right compiler. They're
key in allowing CMake to make a well-informed choice.

To dodge any potential hiccups in the compilation process, it's a good idea to define the compilers using these
variables. This helps to circumvent any possible conflicts or compatibility issues. It's especially handy when multiple
compilers are installed and you need to pick a specific one for a task.

<details>
<summary>Commands for setting the compilers </summary>

- Debian/Ubuntu/MacOS:

  Set your desired compiler (`clang`, `gcc`, etc):

  - Temporarily (only for the current shell)

      Run one of the followings in the terminal:

    - clang

            ```bash
           CC=clang CXX=clang++
            ```

    - gcc

             ```bash
           CC=gcc CXX=g++
            ```

  - Permanent:

      Open `~/.bashrc` using your text editor:

       ```bash
       gedit ~/.bashrc
       ```

      Add `CC` and `CXX` to point to the compilers:

       ```bash
       export CC=clang
       export CXX=clang++
        ```

      Save and close the file.

- Windows:

  - Permanent:

      Run one of the followings in PowerShell:

    - Visual Studio generator and compiler (cl)

             ```powershell
            [Environment]::SetEnvironmentVariable("CC", "cl.exe", "User")
            [Environment]::SetEnvironmentVariable("CXX", "cl.exe", "User")
            refreshenv
            ```

          Set the architecture
          using [vcvarsall](https://docs.microsoft.com/en-us/cpp/build/building-on-the-command-line?view=vs-2019#vcvarsall-syntax):

           vcvarsall.bat x64

            ```powershell
            vcvarsall.bat x64
            ```

    - clang

            ```powershell
           [Environment]::SetEnvironmentVariable("CC", "clang.exe", "User")
           [Environment]::SetEnvironmentVariable("CXX", "clang++.exe", "User")
           refreshenv
            ```

    - gcc

            ```powershell
           [Environment]::SetEnvironmentVariable("CC", "gcc.exe", "User")
           [Environment]::SetEnvironmentVariable("CXX", "g++.exe", "User")
           refreshenv
            ```

  - Temporarily (only for the current shell):

      ```powershell
      $Env:CC="clang.exe"
      $Env:CXX="clang++.exe"
      ```

</details>

### (2) Configure your build

To configure the project, you could use `cmake`, or `ccmake` or `cmake-gui`. Each of them are explained in the
following:

#### (2.a) Configuring via cmake

With Cmake directly:

```bash
cmake -S . -B ./build
```

Cmake will automatically create the `./build` folder if it does not exist, and it wil configure the project.

Instead, if you have CMake version 3.21+, you can use one of the configuration presets that are listed in the
CmakePresets.json file.

```bash
cmake . --preset <configure-preset>
cmake --build
```

#### (2.b) Configuring via ccmake

With the Cmake Curses Dialog Command Line tool:

```bash
ccmake -S . -B ./build
```

Once `ccmake` has finished setting up, press 'c' to configure the project,
press 'g' to generate, and 'q' to quit.

#### (2.c) Configuring via cmake-gui

To use the GUI of the cmake:

2.c.1) Open cmake-gui from the project directory:

``` bash
cmake-gui .
```

2.c.2) Set the build directory:

![build_dir](https://user-images.githubusercontent.com/16418197/82524586-fa48e380-9af4-11ea-8514-4e18a063d8eb.jpg)

2.c.3) Configure the generator:

In cmake-gui, from the upper menu select `Tools/Configure`.

**Warning**: if you have set `CC` and `CXX` always choose the `use default native compilers` option. This picks `CC`
and `CXX`. Don't change the compiler at this stage!

<details>
<summary>Windows - MinGW Makefiles</summary>

Choose MinGW Makefiles as the generator:

<img src="https://user-images.githubusercontent.com/16418197/82769479-616ade80-9dfa-11ea-899e-3a8c31d43032.png" alt="mingw">

</details>

<details>
<summary>Windows - Visual Studio generator and compiler</summary>

You should have already set `C` and `CXX` to `cl.exe`.

Choose "Visual Studio 16 2019" as the generator:

<img src="https://user-images.githubusercontent.com/16418197/82524696-32502680-9af5-11ea-9697-a42000e900a6.jpg" alt="default_vs">

</details>

<details>

<summary>Windows - Visual Studio generator and Clang Compiler</summary>

You should have already set `C` and `CXX` to `clang.exe` and `clang++.exe`.

Choose "Visual Studio 16 2019" as the generator. To tell Visual studio to use `clang-cl.exe`:

- If you use the LLVM that is shipped with Visual Studio: write `ClangCl` under "optional toolset to use".

<img src="https://user-images.githubusercontent.com/16418197/82781142-ae60ac00-9e1e-11ea-8c77-222b005a8f7e.png" alt="visual_studio">

- If you use an external LLVM:
  write [`LLVM_v142`](https://github.com/zufuliu/llvm-utils#llvm-for-visual-studio-2017-and-2019)
  under "optional toolset to use".

<img src="https://user-images.githubusercontent.com/16418197/82769558-b3136900-9dfa-11ea-9f73-02ab8f9b0ca4.png" alt="visual_studio">

</details>
<br/>

2.c.4) Choose the Cmake options and then generate:

![generate](https://user-images.githubusercontent.com/16418197/82781591-c97feb80-9e1f-11ea-86c8-f2748b96f516.png)

### (3) Build the project

Once you have selected all the options you would like to use, you can build the
project (all targets):

```bash
    cmake --build ./build
```

For Visual Studio, give the build configuration (Release, RelWithDeb, Debug, etc) like the following:

```bash
    cmake --build ./build -- /p:configuration=Release
```

### (3) Build the project on Unix from the terminal

To build and manage the project on a Unix-based system, you can utilize the following shell scripts. Each script serves
a specific purpose, allowing you to build, test, and analyze your code efficiently.

1. **`build_and_run.sh`**
    - **Purpose**: Compiles the project and executes the resulting binary.
    - **Usage**: After building, this script will run your code to ensure it operates as expected.
    - **How to Use**: Simply execute the script from the terminal:

      ```bash
      ./build_and_run.sh
      ```

2. **`build_and_test_coverage.sh`**
    - **Purpose**: Builds the project, runs tests, and generates a code coverage report using `gcovr`.
    - **Usage**: This script is particularly useful for verifying that your tests cover a significant portion of your
      codebase.
    - **How to Use**: Run the script to perform the build, testing, and coverage analysis:

      ```bash
      ./build_and_test_coverage.sh
      ```

3. **`build_and_valgrind.sh`**
    - **Purpose**: Compiles the code and runs the Valgrind program to check for memory leaks (using **Memcheck**) or
      performance profiling (using **Callgrind**).
    - **Usage**: This is essential for ensuring memory safety and optimizing code performance.
    - **How to Use**: Execute the script to start Valgrind analysis:

      ```bash
      ./build_and_valgrind.sh
      ```

4. **`cleanHtmlAndCss.sh`**
    - **Purpose**: Cleans up HTML and CSS files, as well as the `cobertura.xml` file, to maintain a tidy project
      environment.
    - **Usage**: Regular cleanup is important to avoid clutter and ensure your project remains organized.
    - **How to Use**: Simply run the script to perform cleanup:

      ```bash
      ./cleanHtmlAndCss.sh
      ```

## Making Scripts Executable

Before using any of the scripts, you need to make them executable. You can do this by running the following commands in
the terminal:

```bash
chmod +x build_and_run.sh
chmod +x build_and_test_coverage.sh
chmod +x build_and_valgrind.sh
chmod +x cleanHtmlAndCss.sh
```

### Running the tests

You can use the `ctest` command run the tests.

```bash
cd ./build
ctest -C Debug
cd ../
```
