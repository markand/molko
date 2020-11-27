# Installation

This page will explain how to build Molko's Adventure and libraries.

## Requirements

The following requirements are necessary to build the project.

- C11 compiler: obviously,
- [CMake][]: the build system,
- [SDL2][]: the main game library,
- [SDL2_image][]: to load images,
- [SDL2_mixer][]: to play sounds and music,
- [SDL2_ttf][]: to open fonts,
- [Jansson][]: only for `mlk-tileset` and `mlk-map` tools.

## Platforms supported

The project has been tested on the following platforms.

- Windows 10: only with [MSYS2][], Visual Studio **is not** supported yet.
- macOS 11.0 (Big Sur): using Xcode 12 and libraries from [brew][],
- Linux: on Alpine Linux (musl) and Arch Linux (glibc).

## C and POSIX features

Both of the API and the game are written using C99 and C11 but not all of the
features from both C versions are required. Here's a list of functionalities
that are required:

### From C99

Features:

- [Compound literals][]
- [inline] keyword

Headers:

- [stdbool.h][]

Functions:

- [snprintf][]
- [vsnprintf][]

### From C11

- [stdnoreturn.h][]

### From POSIX

Expectations:

- We use a few parts of [POSIX][] specification but try to keep its use limited
  as not all platforms are POSIX compliant.
- We assume that `fopen`, `malloc` and friends set errno in case of failures.

Functions:

- [dirname][]
- [getopt][]

Headers:

- [libgen.h][]

Macros:

- `PATH_MAX`: in limits.h.

!!! note
    When a POSIX requirement isn't present it is replaced by a shim
       specified for the platform.

## Building with CMake

Enter the source directory and type the following commands. In this example we
will use the "Unix Makefiles".

    mkdir build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Debug
    make

!!! note
    If you use a CMake generator that supports configuration types (e.g. Xcode,
    Visual Studio) do not set `CMAKE_BUILD_TYPE`.

### Available options

The following CMake options are availble:

`MOLKO_WITH_DOC`
:   Enable documentation (default: On).

`MOLKO_WITH_EXAMPLES`
:   Build examples (default: On).

`MOLKO_WITH_TESTS`
:   Build tests (default: On).

[CMake]: http://cmake.org
[Compound literals]: https://en.cppreference.com/w/c/language/compound_literal
[Jansson]: https://digip.org/jansson
[MSYS2]: https://www.msys2.org
[POSIX]: https://pubs.opengroup.org/onlinepubs/9699919799
[SDL2]: http://libsdl.org
[SDL2_image]: https://www.libsdl.org/projects/SDL_image
[SDL2_mixer]: https://www.libsdl.org/projects/SDL_mixer
[SDL2_ttf]: https://www.libsdl.org/projects/SDL_ttf
[brew]: https://brew.sh
[dirname]: https://pubs.opengroup.org/onlinepubs/9699919799/functions/dirname.html
[getopt]: https://pubs.opengroup.org/onlinepubs/9699919799/functions/getopt.html
[inline]: https://en.cppreference.com/w/c/language/inline
[libgen.h]: https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/libgen.h.html
[snprintf]: https://en.cppreference.com/w/c/io/fprintf
[vsnprintf]: https://en.cppreference.com/w/c/io/fprintf
[stdbool.h]: https://en.cppreference.com/w/c/language/arithmetic_types#Boolean_type
[stdnoreturn.h]: https://en.cppreference.com/w/c/types
