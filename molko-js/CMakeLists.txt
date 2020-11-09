#
# CMakeLists.txt -- CMake build system for molko
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

project(molko-js)

set(
	SOURCES
	${molko-js_SOURCE_DIR}/src/js-animation.c
	${molko-js_SOURCE_DIR}/src/js-animation.h
	${molko-js_SOURCE_DIR}/src/js-clock.c
	${molko-js_SOURCE_DIR}/src/js-clock.h
	${molko-js_SOURCE_DIR}/src/js-event.c
	${molko-js_SOURCE_DIR}/src/js-event.h
	${molko-js_SOURCE_DIR}/src/js-font.c
	${molko-js_SOURCE_DIR}/src/js-font.h
	${molko-js_SOURCE_DIR}/src/js-painter.c
	${molko-js_SOURCE_DIR}/src/js-painter.h
	${molko-js_SOURCE_DIR}/src/js-sprite.c
	${molko-js_SOURCE_DIR}/src/js-sprite.h
	${molko-js_SOURCE_DIR}/src/js-texture.c
	${molko-js_SOURCE_DIR}/src/js-texture.h
	${molko-js_SOURCE_DIR}/src/js-util.c
	${molko-js_SOURCE_DIR}/src/js-util.h
	${molko-js_SOURCE_DIR}/src/js-window.c
	${molko-js_SOURCE_DIR}/src/js-window.h
	${molko-js_SOURCE_DIR}/src/js.c
	${molko-js_SOURCE_DIR}/src/js.h
	${molko-js_SOURCE_DIR}/src/js_p.c
	${molko-js_SOURCE_DIR}/src/js_p.h
	${molko-js_SOURCE_DIR}/src/main.c
)

molko_define_executable(
	TARGET molko-js
	SOURCES ${SOURCES}
	LIBRARIES libcore libui librpg libduktape
)

source_group(src FILES ${SOURCES})
