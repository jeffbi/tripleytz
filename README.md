# tripleytz
An implementation of the classic game "Triple Yahtzee".

## Building From Source
`tripleytz` requires the `CMake` build system, the `Qt6` platform, and a C++17 compatible compiler.

### Linux
To build on Linux at the command line, start in the main project directory, create a `build` subdirectory, run `CMake` then run `make`:
```console
$ mkdir build
$ cd build
build$ cmake -DCMAKE_PREFIX_PATH:PATH=<path-to-qt-config> ..
build$ make
```
Substitute `<path-to-qt-config>` with the path to your Qt CMake configurations. On my system it is `~/Qt/6.4.2/gcc_64`.
