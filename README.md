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
cmake -S <path-to-this-dir> -BUILD_SHARED_LIBS=ON  ...   # Build as shared lib
```

The built library can be found in `<path-to-build-dir>/src`.

The built `dev-demo` executable can be found in `<path-to-build-dir>/exe`.

If you do not wish to build the demo, the option `SB_MAPREDUCE_BUILD_DEMO` can be set at configure time.
If you are using this CMake project as a sub-project of your own build, the demo will not be built by default, but you can override this by setting that option.

## Running the Demo

To run the demo, before configuring, you must have a `<path-to-this-dir>/data` directory.
This directory must in turn have subdirectories `inputs` and `outputs`.
The input file to word count must be in the `inputs` directory.

As of last writing, this defaults to "pg-being-earnest.txt".
This file, along with other example text files, can be downloaded from TODO MIT GITHUB.
The output file will be in `outputs/word_count.txt`.

At configure time, the entire `data` directory will be copied to the CMake build directory you previously specified.
This means, when running the demo, your current working directory must be that build directory.
Thus, to run the demo, assuming the required data directories existed at configure time, you could do:

```shell script
(cd <path-to-build-dir> && exe/dev-demo)
```
## Using the MapReduce library

### Including the Headers

The `libsb-mapreduce` target exposes a public include directory.
Any required headers (whose use will be demonstrated below) can thus be included as follows in your code:
```c++
#include <sb-mapreduce/some-header.h>
```

### Naming Conventions and Namespaces

As shown, the headers use a `lower-kebab-case` style.
The types they expose will use an `UpperCamelCase` style, with their members (and any free-standing functions, etc.) being `lower_snake_case`.
For example:

```c++
#include <sb-mapreduce/some-class.h>

auto c = shiraz::MapReduce::SomeClass{}.some_method();
```

As can be seen, the library is contained inside the `shiraz::MapReduce` namespace.

Thus far, the main user-visible identifiers are within this namespaces; no sub-namespaces required.
That is, other than a `::utils` namespace, which provides some helper functions the user may find useful.
For example, `shiraz::MapReduce::utils::log_file()` will log some lines of a file to stdout.

### Executing a MapReduce Job
TODO
