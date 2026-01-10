#
# CMakeLists.txt -- CMake build system for Molko's Engine
#
# Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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
	ASSETS
	${libmlk-core_SOURCE_DIR}/assets/misc/gamecontrollerdb.txt
)

set(
	SOURCES
	${libmlk-core_SOURCE_DIR}/mlk/core/action-script.c
	${libmlk-core_SOURCE_DIR}/mlk/core/action-stack.c
	${libmlk-core_SOURCE_DIR}/mlk/core/alloc.c
	${libmlk-core_SOURCE_DIR}/mlk/core/animation.c
	${libmlk-core_SOURCE_DIR}/mlk/core/clock.c
	${libmlk-core_SOURCE_DIR}/mlk/core/color.c
	${libmlk-core_SOURCE_DIR}/mlk/core/core.c
	${libmlk-core_SOURCE_DIR}/mlk/core/core_p.h
	${libmlk-core_SOURCE_DIR}/mlk/core/coro.c
	${libmlk-core_SOURCE_DIR}/mlk/core/drawable-stack.c
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
	${libmlk-core_SOURCE_DIR}/mlk/core/vfs-dir.c
	${libmlk-core_SOURCE_DIR}/mlk/core/vfs-zip.c
	${libmlk-core_SOURCE_DIR}/mlk/core/vfs.c
	${libmlk-core_SOURCE_DIR}/mlk/core/vfs_p.h
	${libmlk-core_SOURCE_DIR}/mlk/core/window.c
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
	${libmlk-core_SOURCE_DIR}/mlk/core/vfs-dir.h
	${libmlk-core_SOURCE_DIR}/mlk/core/vfs-zip.h
	${libmlk-core_SOURCE_DIR}/mlk/core/vfs.h
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

if (MLK_WITH_ZIP)
	list(APPEND LIBRARIES ZIP::ZIP)
endif ()

mlk_library(
	NAME libmlk-core
	ASSETS ${ASSETS}
	SOURCES ${SOURCES}
	HEADERS ${HEADERS}
	HEADERS_DIRECTORY mlk/core
	LANGS ${NLS}
	INSTALL
	LIBRARIES
		PUBLIC
			${LIBRARIES}
			${OPENAL_LIBRARY}
			SndFile::SndFile
			SDL3::SDL3-shared
			SDL3::Headers
			SDL3_image::SDL3_image-shared
			SDL3_ttf::SDL3_ttf-shared
			libmlk-minicoro
			libmlk-util
	INCLUDES
		PRIVATE
			$<BUILD_INTERFACE:${libmlk-core_BINARY_DIR}>
		PUBLIC
			$<BUILD_INTERFACE:${libmlk-core_SOURCE_DIR}>
			${OPENAL_INCLUDE_DIR}
)

if (MLK_WITH_JAVASCRIPT)
	set(
		JS_SOURCES
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-animation.c
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-animation.h
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-clock.c
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-clock.h
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-event.c
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-event.h
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-font.c
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-font.h
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-game.c
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-game.h
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-music.c
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-music.h
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-painter.c
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-painter.h
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-sound.c
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-sound.h
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-sprite.c
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-sprite.h
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-state.c
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-state.h
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-texture.c
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-texture.h
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-util.c
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-util.h
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-window.c
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js-window.h
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js.c
		${libmlk-core_SOURCE_DIR}/mlk/core/js/js.h
	)

	mlk_library(
		NAME libmlk-core-js
		SOURCES ${JS_SOURCES}
		INSTALL
		LIBRARIES
			PUBLIC
				libmlk-duktape
				libmlk-core
	)

	source_group(TREE ${libmlk-core_SOURCE_DIR} FILES ${JS_SOURCES})
endif ()

source_group(TREE ${libmlk-core_SOURCE_DIR} FILES ${ASSETS} ${HEADERS} ${SOURCES} ${NLS})
