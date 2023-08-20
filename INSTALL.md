Molko's Engine INSTALL
======================

Installation instructions.

Requirements
============

- C23 compliant compiler.
- [CMake][], Make build system.
- [Jansson][], JSON parsing library.
- [SDL3][], Multimedia library.
- [SDL3_image][], Image loading addon for SDL3.
- [SDL3_mixer][], Audio addon for SDL3.
- [SDL3_ttf][], Fonts addon for SDL3.

Molko's Engine is mostly written in C23 with a very limited POSIX extensions
(including `stat`, `strlcpy`, `fmemopen`) but where support is missing fallback
implementations are provided.

Supported platforms
===================

The project was successfully tested on the following platforms and their
architectures.

- Windows (MinGW-w64, VS2019).
- Linux (musl/amd64, musl/aarch64, glibc/amd64).
- FreeBSD (amd64).
- OpenBSD (amd64).
- NetBSD (amd64).
- macOS (Monterey, Ventura).

Basic installation
==================

Quick install.

	$ tar -xvf molko-x.y.z-tar.xz
	$ cd molko-x.y.z
	$ cmake -S. -Bbuild
	$ cmake --build build
	$ cmake --install build

Available options
=================

The following cmake options as available:

- `MLK_WITH_DOXYGEN`: enable doxygen documentation (default: On, requires
  doxygen).
- `MLK_WITH_EXAMPLES`: enable examples (default: on).
- `MLK_WITH_NLS`: enable i18n (default: on, requires gettext).
- `MLK_WITH_TESTS`: enable unit tests (default: on).
- `MLK_WITH_TESTS_GRAPHICAL`: enable unit tests that require a window
  context(default: on).
- `MLK_WITH_CMAKEDIR`: root directory for CMake files (default: LIBDIR/cmake).

Platform: macOS
---------------

The recommended way to build under macOS is to install dependencies through
[brew][] and compile using XCode or command line generated makefiles.

You will need the following packages:

- *jansson*
- *sdl3*
- *sdl3_image*
- *sdl3_mixer*
- *sdl3_ttf*

Platform: Windows
-----------------

On Windows, [MSYS2][] and Visual Studio are supported.

### MSYS2

Once you have MSYS2 installed, simply install the following packages from the
appropriate MinGW shell prior to the chapter above.

- *mingw-w64-clang-x86_64-SDL3*
- *mingw-w64-clang-x86_64-SDL3_image*
- *mingw-w64-clang-x86_64-SDL3_mixer*
- *mingw-w64-clang-x86_64-SDL3_ttf*
- *mingw-w64-clang-x86_64-jansson*

Note: replace `x86_64` with `i686` if you have a deprecated system or if you
      have issues while debugging (MinGW-w64 and/or gdb have known issues in
      this area).

Remove `clang-` prefix if you need to use gcc for some reasons.

### Visual Studio

On Visual Studio you will need to download or build libraries yourself. It is
recommended to install all of them in a same directory and hierarchy as
following:

- *dir/bin*
- *dir/lib*
- *dir/include*

Make sure *dir/bin* is in your `PATH` and adjust `CMAKE_PREFIX_PATH` before
running `cmake`.

[brew]: http://brew.sh
[CMake]: http://cmake.org
[Jansson]: http://www.digip.org/jansson
[MSYS2]: http://www.msys2.org
[SDL3]: http://libsdl.org
[SDL3_image]: https://www.libsdl.org/projects/SDL_image
[SDL3_mixer]: https://www.libsdl.org/projects/SDL_mixer
[SDL3_ttf]: https://www.libsdl.org/projects/SDL_ttf
[gettext]: https://www.gnu.org/software/gettext
