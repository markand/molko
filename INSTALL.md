Molko's Adventure INSTALL
=========================

Installation instructions.

Requirements
------------

- C11 compliant compiler,
- POSIX system (make, ar, shell),
- [pkg-config][], Helper for libraries.
- [Jansson][], JSON parsing library,
- [SDL2][], Multimedia library,
- [SDL2_image][], Image loading addon for SDL2,
- [SDL2_ttf][], Fonts addon for SDL2,
- [SDL2_mixer][], Audio addon for SDL2.

Basic installation
------------------

Quick install.

	$ tar xvzf molko-x.y.z-tar.xz
	$ cd molko-x.y.z
	$ make
	# sudo make install
	$ molko

Available general make targets:

- `all`: (default): build only molko.
- `tools`: build tools (e.g. molko-map).
- `tests`: build tests and run them.
- `everything`: build molko, tools and tests without running them.
- `run`: run molko with data installed in the source tree.

Direct use in source tree
-------------------------

Examples and `mlk-adventure` executable searches for data into a specific
directory, when building those are not already discoverable because targets are
not already installed.

You need to install at least the data once using `install-data` target, you can
do this without root access by using a temporary directory and set the
`MLK_ROOT` environment variable.

The directory `out` is recommended, see below.

	$ make DESTDIR=out install-data
	$ MLK_ROOT=out ./mlk-adventure/mlk-adventure
	(or to run examples:)
	$ MLK_ROOT=out ./examples/example-action/main

There is a special target `run` that automatically create the diretory `out`
with all data and binaries installed and runs `mlk-adventure`.

Platform: Windows
-----------------

On Windows, only [MSYS2][] is supported.

The code may compile on Microsoft Visual Studio but no support for it is
provided as it is not opensource and not standard compliant.

Once you have MSYS2 installed, simply install the following packages from the
appropriate MinGW shell prior to the chapter above.

- *make*
- *mingw-w64-x86_64-pkg-config*
- *mingw-w64-x86_64-gcc*
- *mingw-w64-x86_64-SDL2*
- *mingw-w64-x86_64-SDL2_image*
- *mingw-w64-x86_64-SDL2_mixer*
- *mingw-w64-x86_64-SDL2_ttf*
- *mingw-w64-x86_64-jansson* (only for mlk-map/mlk-tileset tools)

Note: replace `x86_64` with `i686` if you have a deprecated system or if you
      have issues while debugging (MinGW-w64 and/or gdb have known issues in
      this area).

[Jansson][]: http://www.digip.org/jansson
[SDL2]: http://libsdl.org
[SDL2_image]: https://www.libsdl.org/projects/SDL_image
[SDL2_ttf]: https://www.libsdl.org/projects/SDL_ttf
[SDL2_mixer]: https://www.libsdl.org/projects/SDL_mixer
[MSYS2]: http://www.msys2.org
[pkg-config]: http://pkgconf.org
