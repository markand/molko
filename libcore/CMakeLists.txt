#
# CMakeLists.txt -- CMake build system for libcore
#
# Copyright (c) 2020 David Demelier <markand@malikania.fr>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#

project(libcore)

set(
	SOURCES
	${libcore_SOURCE_DIR}/core/action.c
	${libcore_SOURCE_DIR}/core/action.h
	${libcore_SOURCE_DIR}/core/animation.c
	${libcore_SOURCE_DIR}/core/animation.h
	${libcore_SOURCE_DIR}/core/clock.c
	${libcore_SOURCE_DIR}/core/clock.h
	${libcore_SOURCE_DIR}/core/color.h
	${libcore_SOURCE_DIR}/core/core.c
	${libcore_SOURCE_DIR}/core/core.h
	${libcore_SOURCE_DIR}/core/drawable.c
	${libcore_SOURCE_DIR}/core/drawable.h
	${libcore_SOURCE_DIR}/core/error.c
	${libcore_SOURCE_DIR}/core/error.h
	${libcore_SOURCE_DIR}/core/error_p.h
	${libcore_SOURCE_DIR}/core/event.c
	${libcore_SOURCE_DIR}/core/event.h
	${libcore_SOURCE_DIR}/core/font.c
	${libcore_SOURCE_DIR}/core/font.h
	${libcore_SOURCE_DIR}/core/game.c
	${libcore_SOURCE_DIR}/core/game.h
	${libcore_SOURCE_DIR}/core/image.c
	${libcore_SOURCE_DIR}/core/image.h
	${libcore_SOURCE_DIR}/core/inhibit.c
	${libcore_SOURCE_DIR}/core/inhibit.h
	${libcore_SOURCE_DIR}/core/key.h
	${libcore_SOURCE_DIR}/core/maths.c
	${libcore_SOURCE_DIR}/core/maths.h
	${libcore_SOURCE_DIR}/core/mouse.h
	${libcore_SOURCE_DIR}/core/painter.c
	${libcore_SOURCE_DIR}/core/painter.h
	${libcore_SOURCE_DIR}/core/panic.c
	${libcore_SOURCE_DIR}/core/panic.h
	${libcore_SOURCE_DIR}/core/plat.h
	${libcore_SOURCE_DIR}/core/rbuf.c
	${libcore_SOURCE_DIR}/core/rbuf.h
	${libcore_SOURCE_DIR}/core/save.c
	${libcore_SOURCE_DIR}/core/save.h
	${libcore_SOURCE_DIR}/core/script.c
	${libcore_SOURCE_DIR}/core/script.h
	${libcore_SOURCE_DIR}/core/sound.c
	${libcore_SOURCE_DIR}/core/sound.h
	${libcore_SOURCE_DIR}/core/sprite.c
	${libcore_SOURCE_DIR}/core/sprite.h
	${libcore_SOURCE_DIR}/core/state.c
	${libcore_SOURCE_DIR}/core/state.h
	${libcore_SOURCE_DIR}/core/sys.c
	${libcore_SOURCE_DIR}/core/sys.h
	${libcore_SOURCE_DIR}/core/texture.c
	${libcore_SOURCE_DIR}/core/texture.h
	${libcore_SOURCE_DIR}/core/texture_p.h
	${libcore_SOURCE_DIR}/core/trace.c
	${libcore_SOURCE_DIR}/core/trace.h
	${libcore_SOURCE_DIR}/core/util.c
	${libcore_SOURCE_DIR}/core/util.h
	${libcore_SOURCE_DIR}/core/wait.c
	${libcore_SOURCE_DIR}/core/wait.h
	${libcore_SOURCE_DIR}/core/window.c
	${libcore_SOURCE_DIR}/core/window.h
	${libcore_SOURCE_DIR}/core/window_p.h
)

molko_define_library(
	TARGET libcore
	SOURCES ${SOURCES}
	LIBRARIES
		libsqlite
		SDL2::SDL2
		SDL2::image
		SDL2::mixer
		SDL2::ttf
	PRIVATE_FLAGS
		BINDIR="${CMAKE_INSTALL_BINDIR}"
		PREFIX="${CMAKE_INSTALL_PREFIX}"
		SHAREDIR="${CMAKE_INSTALL_DATADIR}"
	PUBLIC_FLAGS
		_XOPEN_SOURCE=700
	PUBLIC_INCLUDES
		$<BUILD_INTERFACE:${libcore_SOURCE_DIR}>
)

source_group(core FILES ${SOURCES})
