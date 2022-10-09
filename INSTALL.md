Molko's Engine INSTALL
======================

Installation instructions.

Requirements
============

- C11 compliant compiler.
- [GNU Make][], Make build system.
- [Jansson][], JSON parsing library.
- [SDL2][], Multimedia library.
- [SDL2_image][], Image loading addon for SDL2.
- [SDL2_mixer][], Audio addon for SDL2.
- [SDL2_ttf][], Fonts addon for SDL2.
- [zstd][], For compression (optional).
- [libzip][], For ZIP vfs support (optional).

Molko's Engine is mostly written in pure C99 with a very limited POSIX
extensions (including `stat`, `strlcpy`, `fmemopen`) but where support is
missing fallback implementations are provided.

Supported platforms
===================

The project was successfully tested on the following platforms and their
architectures.

- Windows (MinGW-w64, VS2019).
- Linux (musl/amd64, musl/aarch64, glibc/amd64).
- FreeBSD (amd64).
- OpenBSD (amd64).
- macOS

Basic installation
==================

Quick install.

	$ bsdtar -xvf molko-x.y.z-tar.xz
	$ cd molko-x.y.z
	$ make
	# sudo make install

Available options
=================

The following make variables as available:

- `WITH_ZSTD`: Enable map and tileset compression through [zstd][] (default:
  yes).
- `WITH_ZIP`: Enable ZIP file support through VFS API (default: yes).
- `WITH_DEBUG`: Disable optimizations and turn on debug symbols (default: no).

The following make variables changes installation directories:

- `PREFIX`: Root install path (default: /usr/local).
- `LIBDIR`: Libraries directory (default: $PREFIX/lib).
- `INCDIR`: C header directory (default: $PREFIX/include).

Booleans options can be set to `yes` or `no` (case sensitive).

Platform: macOS
---------------

The recommended way to build under macOS is to install dependencies through
[brew][] and compile using XCode or command line generated makefiles.

You will need the following packages:

- jansson
- sdl2
- sdl2_image
- sdl2_mixer
- sdl2_ttf
- zstd (only if `WITH_ZSTD` is enabled)
- libzip (only if `WITH_ZIP` is enabled)

Platform: Windows
-----------------

On Windows, [MSYS2][] and Visual Studio are supported.

### MSYS2

Once you have MSYS2 installed, simply install the following packages from the
appropriate MinGW shell prior to the chapter above.

- *make*
- *mingw-w64-clang-x86_64-gcc*
- *mingw-w64-clang-x86_64-SDL2*
- *mingw-w64-clang-x86_64-SDL2_image*
- *mingw-w64-clang-x86_64-SDL2_mixer*
- *mingw-w64-clang-x86_64-SDL2_ttf*
- *mingw-w64-clang-x86_64-jansson*
- *mingw-w64-clang-x86_64-zstd* (only if `WITH_ZSTD` is enabled)
- *mingw-w64-clang-x86_64-zip* (only if `WITH_ZSTD` is enabled)

Note: replace `x86_64` with `i686` if you have a deprecated system or if you
      have issues while debugging (MinGW-w64 and/or gdb have known issues in
      this area).

Remove `clang-` prefix if you need to use gcc for some reasons.

### Visual Studio

On Visual Studio you will need to download or build libraries yourself. It is
recommended to install all of them in a same directory and hierarchy as
following:

- <dir>/bin
- <dir>/lib
- <dir>/include

Building with Visual Studio is only supported through `clang-cl` which should be
installed as individual component from the Visual Studio installer. You also
need a POSIX compliant toolset such as [MSYS2][] and add it to your path.

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
