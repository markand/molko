#
# CMakeLists.txt -- CMake build system for libmlk-core
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

project(libmlk-core)

set(
	PO
	${libmlk-core_SOURCE_DIR}/nls/fr.po
	${libmlk-core_SOURCE_DIR}/nls/libmlk-core.pot
)

set(
	SOURCES
	${libmlk-core_SOURCE_DIR}/core/action.c
	${libmlk-core_SOURCE_DIR}/core/action.h
	${libmlk-core_SOURCE_DIR}/core/alloc.c
	${libmlk-core_SOURCE_DIR}/core/alloc.h
	${libmlk-core_SOURCE_DIR}/core/animation.c
	${libmlk-core_SOURCE_DIR}/core/animation.h
	${libmlk-core_SOURCE_DIR}/core/clock.c
	${libmlk-core_SOURCE_DIR}/core/clock.h
	${libmlk-core_SOURCE_DIR}/core/color.h
	${libmlk-core_SOURCE_DIR}/core/core.c
	${libmlk-core_SOURCE_DIR}/core/core.h
	${libmlk-core_SOURCE_DIR}/core/core_p.h
	${libmlk-core_SOURCE_DIR}/core/drawable.c
	${libmlk-core_SOURCE_DIR}/core/drawable.h
	${libmlk-core_SOURCE_DIR}/core/error.c
	${libmlk-core_SOURCE_DIR}/core/error.h
	${libmlk-core_SOURCE_DIR}/core/event.c
	${libmlk-core_SOURCE_DIR}/core/event.h
	${libmlk-core_SOURCE_DIR}/core/font.c
	${libmlk-core_SOURCE_DIR}/core/font.h
	${libmlk-core_SOURCE_DIR}/core/game.c
	${libmlk-core_SOURCE_DIR}/core/game.h
	${libmlk-core_SOURCE_DIR}/core/image.c
	${libmlk-core_SOURCE_DIR}/core/image.h
	${libmlk-core_SOURCE_DIR}/core/inhibit.h
	${libmlk-core_SOURCE_DIR}/core/key.h
	${libmlk-core_SOURCE_DIR}/core/maths.c
	${libmlk-core_SOURCE_DIR}/core/maths.h
	${libmlk-core_SOURCE_DIR}/core/mouse.h
	${libmlk-core_SOURCE_DIR}/core/music.c
	${libmlk-core_SOURCE_DIR}/core/music.h
	${libmlk-core_SOURCE_DIR}/core/painter.c
	${libmlk-core_SOURCE_DIR}/core/painter.h
	${libmlk-core_SOURCE_DIR}/core/panic.c
	${libmlk-core_SOURCE_DIR}/core/panic.h
	${libmlk-core_SOURCE_DIR}/core/script.c
	${libmlk-core_SOURCE_DIR}/core/script.h
	${libmlk-core_SOURCE_DIR}/core/sound.c
	${libmlk-core_SOURCE_DIR}/core/sound.h
	${libmlk-core_SOURCE_DIR}/core/sprite.c
	${libmlk-core_SOURCE_DIR}/core/sprite.h
	${libmlk-core_SOURCE_DIR}/core/state.c
	${libmlk-core_SOURCE_DIR}/core/state.h
	${libmlk-core_SOURCE_DIR}/core/sys.c
	${libmlk-core_SOURCE_DIR}/core/sys.h
	${libmlk-core_SOURCE_DIR}/core/sysconfig.h.in
	${libmlk-core_SOURCE_DIR}/core/texture.c
	${libmlk-core_SOURCE_DIR}/core/texture.h
	${libmlk-core_SOURCE_DIR}/core/texture_p.h
	${libmlk-core_SOURCE_DIR}/core/trace.c
	${libmlk-core_SOURCE_DIR}/core/trace.h
	${libmlk-core_SOURCE_DIR}/core/translate.c
	${libmlk-core_SOURCE_DIR}/core/translate.h
	${libmlk-core_SOURCE_DIR}/core/util.c
	${libmlk-core_SOURCE_DIR}/core/util.h
	${libmlk-core_SOURCE_DIR}/core/window.c
	${libmlk-core_SOURCE_DIR}/core/window.h
	${libmlk-core_SOURCE_DIR}/core/window_p.h
	${libmlk-core_SOURCE_DIR}/core/zfile.c
	${libmlk-core_SOURCE_DIR}/core/zfile.h
)

configure_file(
	${libmlk-core_SOURCE_DIR}/core/sysconfig.h.in
	${libmlk-core_BINARY_DIR}/sysconfig.h
)

if (MOLKO_WITH_NLS AND Intl_FOUND)
	list(APPEND LIBS ${Intl_LIBRARIES})
	list(APPEND INCS ${Intl_INCLUDE_DIRS})
endif ()

molko_define_library(
	TARGET libmlk-core
	SOURCES ${SOURCES} ${ASSETS} ${PO}
	TRANSLATIONS fr
	LIBRARIES
		PUBLIC
			m
			${LIBS}
			libcompat
			libsqlite
			SDL2::SDL2
			SDL2::image
			SDL2::mixer
			SDL2::ttf
	INCLUDES
		PUBLIC
			${INCS}
			$<BUILD_INTERFACE:${libmlk-core_SOURCE_DIR}>
			$<BUILD_INTERFACE:${libmlk-core_BINARY_DIR}>
)

source_group(TREE ${libmlk-core_SOURCE_DIR} FILES ${ASSETS} ${PO} ${SOURCES})
