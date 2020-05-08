# MapReduce in C++
For fun. For learning.

## Build

This is a CMake project. Do the normal CMake thing. Minimum version required as of last writing is 3.16. Please check `CMakeLists.txt` for the final word.

To build, for example:

```shell script
cmake -S <path-to-this-dir> -B <path-to-a-build-dir-of-your-choosing>
cmake --build <path-to-that-same-build-dir>
```

The CMake targets are the MapReduce library itself `libsb-mapreduce`; and a demo `dev-demo` that runs a word count job.

The library can be built as shared (dynamic) or static using the `BUILD_SHARED_LIBS` switch at configure time, defaulting to static.

```shell script
cmake -S <path-to-this-dir> -BUILD_SHARED_LIBS=ON     # Build as shared lib
```

The built library can be found in `<path-to-build-dir>/src`.

The built `dev_demo` executable can be found in `<path-to-build-dir>/exe`.

## Running the Demo

To run the demo, before configuring, you must have a `<path-to-this-dir>/data` directory.
This directory must in turn have subdirectories `inputs` and `outputs`.
The input file to word count must be in the `inputs` directory.

As of last writing, this defaults to "pg-being-earnest.txt".
This file, along with other example text files, can be downloaded from TODO MIT GITHUB.
The output file will be in `outputs/word_count.txt`.

When running the demo, your current working directory must be the CMake build directory you previously specified.

