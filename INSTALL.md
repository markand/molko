Molko's Adventure INSTALL
=========================

Installation instructions.

Requirements
------------

- C11 compliant compiler.
- [CMake][], CMake build system.
- [Jansson][], JSON parsing library.
- [SDL2][], Multimedia library.
- [SDL2_image][], Image loading addon for SDL2.
- [SDL2_mixer][], Audio addon for SDL2.
- [SDL2_ttf][], Fonts addon for SDL2,
- [gettext][], For translations (optional).
- [zstd][], For compression (optional).

Molko's Adventure is mostly written in pure C11 with a very limited POSIX
extensions (including `stat`, `strlcpy`, `fmemopen`) but where support is
missing fallback implementations are provided.

Supported platforms
-------------------

The project was successfully tested on the following platforms and their
architectures.

- Windows (MinGW-w64, VS2019).
- Linux (musl/amd64, musl/aarch64, glibc/amd64).
- FreeBSD (amd64).
- macOS Big Sur.

Basic installation
------------------

Quick install.

	$ tar xvzf molko-x.y.z-tar.xz
	$ cd molko-x.y.z
	$ cmake -S. -Bbuild
	$ cmake --build build --target all
	# cmake --build build --target install
	$ mlk-adventure

Available options
-----------------

The following options are available:

- `MLK_WITH_NLS`: Enable Native Language Support 
- `MLK_WITH_ZSTD`: Enable map and tileset compression through [zstd][] (default:
  on).
- `MLK_WITH_TESTS`: Enable unit tests (default: off).
- `MLK_WITH_EXAMPLES`: Enable sample programs.

Direct use in source tree
-------------------------

Examples and `mlk-adventure` executables are written outside of the CMake build
tree and generated directly in the source tree. This is designed to search data
without installing.

Platform: Linux and BSD
-----------------------

Install dependencies using the package manager provided and use CMake to
generate Makefiles.

Platform: macOS
---------------

The recommended way to build under macOS is to install dependencies through
[brew][] and compile using XCode or command line generated makefiles.

You will need the following packages:

- gettext (only if `MLK_WITH_NLS` is set)
- jansson (only for mlk-map/mlk-tileset tools)
- sdl2
- sdl2_image
- sdl2_mixer
- sdl2_ttf
- zstd (only if `MLK_WITH_ZSTD` is set)

Platform: Windows
-----------------

On Windows, [MSYS2][] and Visual Studio are supported.

### MSYS2

Once you have MSYS2 installed, simply install the following packages from the
appropriate MinGW shell prior to the chapter above.

- *make*
- *mingw-w64-x86_64-gcc*
- *mingw-w64-x86_64-SDL2*
- *mingw-w64-x86_64-SDL2_image*
- *mingw-w64-x86_64-SDL2_mixer*
- *mingw-w64-x86_64-SDL2_ttf*
- *mingw-w64-x86_64-jansson* (only for mlk-map/mlk-tileset tools)
- *mingw-w64-x86_64-zstd* (only if `MLK_WITH_ZSTD` is set)

Note: replace `x86_64` with `i686` if you have a deprecated system or if you
      have issues while debugging (MinGW-w64 and/or gdb have known issues in
      this area).

### Visual Studio

On Visual Studio you will need to download or build libraries yourself. It is
recommended to install all of them in a same directory and hierarchy as
following:

- <dir>/bin
- <dir>/lib
- <dir>/include

Then adding `<dir>/bin` to *PATH* and `<dir>` to *CMAKE_PREFIX_PATH* will allow
CMake to find libraries.

[brew][]: http://brew.sh
[CMake][]: http://cmake.org
[Jansson][]: http://www.digip.org/jansson
[MSYS2]: http://www.msys2.org
[SDL2]: http://libsdl.org
[SDL2_image]: https://www.libsdl.org/projects/SDL_image
[SDL2_mixer]: https://www.libsdl.org/projects/SDL_mixer
[SDL2_ttf]: https://www.libsdl.org/projects/SDL_ttf
[gettext]: https://www.gnu.org/software/gettext
[zstd]: http://facebook.github.io/zstd
