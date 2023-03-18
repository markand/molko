#
# CMakeLists.txt -- CMake build system for Molko's Engine
#
# Copyright (c) 2020-2023 David Demelier <markand@malikania.fr>
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
	SOURCES
	${libmlk-core_SOURCE_DIR}/mlk/core/action-script.c
	${libmlk-core_SOURCE_DIR}/mlk/core/action-stack.c
	${libmlk-core_SOURCE_DIR}/mlk/core/action.c
	${libmlk-core_SOURCE_DIR}/mlk/core/alloc.c
	${libmlk-core_SOURCE_DIR}/mlk/core/animation.c
	${libmlk-core_SOURCE_DIR}/mlk/core/clock.c
	${libmlk-core_SOURCE_DIR}/mlk/core/core.c
	${libmlk-core_SOURCE_DIR}/mlk/core/core.c
	${libmlk-core_SOURCE_DIR}/mlk/core/core_p.h
	${libmlk-core_SOURCE_DIR}/mlk/core/drawable-stack.c
	${libmlk-core_SOURCE_DIR}/mlk/core/drawable.c
	${libmlk-core_SOURCE_DIR}/mlk/core/err.c
	${libmlk-core_SOURCE_DIR}/mlk/core/event.c
	${libmlk-core_SOURCE_DIR}/mlk/core/font.c
	${libmlk-core_SOURCE_DIR}/mlk/core/game.c
	${libmlk-core_SOURCE_DIR}/mlk/core/gamepad.c
	${libmlk-core_SOURCE_DIR}/mlk/core/image.c
	${libmlk-core_SOURCE_DIR}/mlk/core/maths.c
	${libmlk-core_SOURCE_DIR}/mlk/core/music.c
	${libmlk-core_SOURCE_DIR}/mlk/core/painter.c
	${libmlk-core_SOURCE_DIR}/mlk/core/panic.c
	${libmlk-core_SOURCE_DIR}/mlk/core/sound.c
	${libmlk-core_SOURCE_DIR}/mlk/core/sprite.c
	${libmlk-core_SOURCE_DIR}/mlk/core/state.c
	${libmlk-core_SOURCE_DIR}/mlk/core/sys.c
	${libmlk-core_SOURCE_DIR}/mlk/core/texture.c
	${libmlk-core_SOURCE_DIR}/mlk/core/trace.c
	${libmlk-core_SOURCE_DIR}/mlk/core/util.c
)

set(
	HEADERS
	${libmlk-core_SOURCE_DIR}/mlk/core/action-script.h
	${libmlk-core_SOURCE_DIR}/mlk/core/action-stack.h
	${libmlk-core_SOURCE_DIR}/mlk/core/action.h
	${libmlk-core_SOURCE_DIR}/mlk/core/alloc.h
	${libmlk-core_SOURCE_DIR}/mlk/core/animation.h
	${libmlk-core_SOURCE_DIR}/mlk/core/clock.h
	${libmlk-core_SOURCE_DIR}/mlk/core/color.h
	${libmlk-core_SOURCE_DIR}/mlk/core/core.h
	${libmlk-core_SOURCE_DIR}/mlk/core/drawable-stack.h
	${libmlk-core_SOURCE_DIR}/mlk/core/drawable.h
	${libmlk-core_SOURCE_DIR}/mlk/core/err.h
	${libmlk-core_SOURCE_DIR}/mlk/core/event.h
	${libmlk-core_SOURCE_DIR}/mlk/core/font.h
	${libmlk-core_SOURCE_DIR}/mlk/core/game.h
	${libmlk-core_SOURCE_DIR}/mlk/core/gamepad.h
	${libmlk-core_SOURCE_DIR}/mlk/core/image.h
	${libmlk-core_SOURCE_DIR}/mlk/core/key.h
	${libmlk-core_SOURCE_DIR}/mlk/core/maths.h
	${libmlk-core_SOURCE_DIR}/mlk/core/mouse.h
	${libmlk-core_SOURCE_DIR}/mlk/core/music.h
	${libmlk-core_SOURCE_DIR}/mlk/core/painter.h
	${libmlk-core_SOURCE_DIR}/mlk/core/panic.h
	${libmlk-core_SOURCE_DIR}/mlk/core/sound.h
	${libmlk-core_SOURCE_DIR}/mlk/core/sprite.h
	${libmlk-core_SOURCE_DIR}/mlk/core/state.h
	${libmlk-core_SOURCE_DIR}/mlk/core/sys.h
	${libmlk-core_SOURCE_DIR}/mlk/core/sys_p.h
	${libmlk-core_SOURCE_DIR}/mlk/core/texture.h
	${libmlk-core_SOURCE_DIR}/mlk/core/texture_p.h
	${libmlk-core_SOURCE_DIR}/mlk/core/trace.h
	${libmlk-core_SOURCE_DIR}/mlk/core/util.h
	${libmlk-core_SOURCE_DIR}/mlk/core/window.h
	${libmlk-core_SOURCE_DIR}/mlk/core/window_p.h
)

set(
	NLS
	${libmlk-core_SOURCE_DIR}/nls/fr.po
)

if (MLK_WITH_NLS)
	list(APPEND LIBRARIES ${Intl_LIBRARIES})
endif ()

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	list(APPEND LIBRARIES shlwapi)
endif ()

if (M_LIBRARY)
	list(APPEND LIBRARIES ${M_LIBRARY})
endif ()

if (MLK_WITH_NLS)
	list(APPEND LIBRARIES Intl::Intl)
endif ()

mlk_library(
	NAME libmlk-core
	SOURCES ${HEADERS} ${NLS} ${SOURCES}
	HEADERS ${HEADERS}
	HEADERS_DIRECTORY mlk/core
	LANGS ${NLS}
	INSTALL
	LIBRARIES
		PUBLIC
			${LIBRARIES}
			${OPENAL_LIBRARY}
			SndFile::SndFile
			SDL2::SDL2
			SDL2::image
			SDL2::ttf
			libmlk-util
	INCLUDES
		PUBLIC
			$<BUILD_INTERFACE:${libmlk-core_SOURCE_DIR}>
			${OPENAL_INCLUDE_DIR}
)

source_group(TREE ${libmlk-core_SOURCE_DIR} FILES ${SOURCES} ${NLS})
