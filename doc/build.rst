====================
Building from source
====================

This page will explain how to build Molko's Adventure and libraries.

Requirements
------------

The following requirements are necessary to build the project.

- C11 compiler: obviously,
- cmake_: the build system,
- `SDL2`_: the main game library,
- `SDL2_image`_: to load images,
- `SDL2_mixer`_: to play sounds and music,
- `SDL2_ttf`_: to open fonts,
- `Jansson`_: only for ``mlk-tileset`` and ``mlk-map`` tools.

Platforms supported
-------------------

The project has been tested on the following platforms.

- Windows 10: only with `MSYS2`_, Visual Studio **is not** supported yet.
- macOS 11.0 (Big Sur): using Xcode 12 and libraries from `brew`_,
- Linux: on Alpine Linux (musl) and Arch Linux.

C and POSIX features
--------------------

Both of the API and the game are written using C99 and C11 but not all of the
features from both C versions are required. Here's a list of functionalities
that are required:

From C99:

- `Compound literals`_,
- `stdbool.h`_ header,
- `snprintf`_ function and friends,
- `inline`_ keyword.

From C11:

- `stdnoreturn.h`_ header.

From POSIX:

We use a few parts of POSIX_ specification but try to keep its use limited as
not all platforms are POSIX compliant.

- `getopt(3)`_: to parse arguments,
- We assume that ``fopen``, ``malloc`` and friends set errno in case of failures.

.. note:: When a POSIX requirement isn't present it is replaced by a shim
   specified for the platform.

.. _Compound literals: https://en.cppreference.com/w/c/language/compound_literal
.. _Jansson: https://digip.org/jansson
.. _MSYS2: https://www.msys2.org
.. _POSIX: https://pubs.opengroup.org/onlinepubs/9699919799
.. _SDL2: http://libsdl.org
.. _SDL2_image: https://www.libsdl.org/projects/SDL_image
.. _SDL2_mixer: https://www.libsdl.org/projects/SDL_mixer
.. _SDL2_ttf: https://www.libsdl.org/projects/SDL_ttf
.. _brew: https://brew.sh
.. _cmake: http://cmake.org
.. _getopt(3): https://pubs.opengroup.org/onlinepubs/9699919799/functions/getopt.html
.. _inline: https://en.cppreference.com/w/c/language/inline
.. _snprintf: https://en.cppreference.com/w/c/io/fprintf
.. _stdbool.h: https://en.cppreference.com/w/c/language/arithmetic_types#Boolean_type
.. _stdnoreturn.h: https://en.cppreference.com/w/c/types
