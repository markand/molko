#
# CMakeLists.txt -- CMake build system for Molko's Engine
#
# Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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

project(libmlk-core-js)

set(
	SOURCES
	${libmlk-core-js_SOURCE_DIR}/core/js-action-stack.c
	${libmlk-core-js_SOURCE_DIR}/core/js-action-stack.h
	${libmlk-core-js_SOURCE_DIR}/core/js-action.c
	${libmlk-core-js_SOURCE_DIR}/core/js-action.h
	${libmlk-core-js_SOURCE_DIR}/core/js-animation.c
	${libmlk-core-js_SOURCE_DIR}/core/js-animation.h
	${libmlk-core-js_SOURCE_DIR}/core/js-clock.c
	${libmlk-core-js_SOURCE_DIR}/core/js-clock.h
	${libmlk-core-js_SOURCE_DIR}/core/js-color.c
	${libmlk-core-js_SOURCE_DIR}/core/js-color.h
	${libmlk-core-js_SOURCE_DIR}/core/js-core.c
	${libmlk-core-js_SOURCE_DIR}/core/js-core.h
	${libmlk-core-js_SOURCE_DIR}/core/js-drawable-stack.c
	${libmlk-core-js_SOURCE_DIR}/core/js-drawable-stack.h
	${libmlk-core-js_SOURCE_DIR}/core/js-drawable.c
	${libmlk-core-js_SOURCE_DIR}/core/js-drawable.h
	${libmlk-core-js_SOURCE_DIR}/core/js-event.c
	${libmlk-core-js_SOURCE_DIR}/core/js-event.h
	${libmlk-core-js_SOURCE_DIR}/core/js-font.c
	${libmlk-core-js_SOURCE_DIR}/core/js-font.h
	${libmlk-core-js_SOURCE_DIR}/core/js-game.c
	${libmlk-core-js_SOURCE_DIR}/core/js-game.h
	${libmlk-core-js_SOURCE_DIR}/core/js-music.c
	${libmlk-core-js_SOURCE_DIR}/core/js-music.h
	${libmlk-core-js_SOURCE_DIR}/core/js-painter.c
	${libmlk-core-js_SOURCE_DIR}/core/js-painter.h
	${libmlk-core-js_SOURCE_DIR}/core/js-panic.c
	${libmlk-core-js_SOURCE_DIR}/core/js-panic.h
	${libmlk-core-js_SOURCE_DIR}/core/js-sound.c
	${libmlk-core-js_SOURCE_DIR}/core/js-sound.h
	${libmlk-core-js_SOURCE_DIR}/core/js-sprite.c
	${libmlk-core-js_SOURCE_DIR}/core/js-sprite.h
	${libmlk-core-js_SOURCE_DIR}/core/js-state.c
	${libmlk-core-js_SOURCE_DIR}/core/js-state.h
	${libmlk-core-js_SOURCE_DIR}/core/js-texture.c
	${libmlk-core-js_SOURCE_DIR}/core/js-texture.h
	${libmlk-core-js_SOURCE_DIR}/core/js-trace.c
	${libmlk-core-js_SOURCE_DIR}/core/js-trace.h
	${libmlk-core-js_SOURCE_DIR}/core/js-window.c
	${libmlk-core-js_SOURCE_DIR}/core/js-window.h
)

mlk_library(
	NAME libmlk-core-js
	SOURCES ${SOURCES}
	LIBRARIES libmlk-core libmlk-duktape
	INCLUDES
		PUBLIC $<BUILD_INTERFACE:${libmlk-core-js_SOURCE_DIR}>
)

source_group(TREE ${libmlk-core-js_SOURCE_DIR} FILES ${SOURCES})
