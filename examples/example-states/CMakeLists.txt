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

project(example-states)

set(
	SOURCES
	${example-states_SOURCE_DIR}/example-states.c
	${example-states_SOURCE_DIR}/state-menu.c
	${example-states_SOURCE_DIR}/state-menu.h
	${example-states_SOURCE_DIR}/state-play.c
	${example-states_SOURCE_DIR}/state-play.h
	${example-states_SOURCE_DIR}/state-splash.c
	${example-states_SOURCE_DIR}/state-splash.h
)

set(
	ASSETS
	${example-states_SOURCE_DIR}/assets/fonts/breamcatcher.otf
	${example-states_SOURCE_DIR}/assets/fonts/zenda.ttf
	${example-states_SOURCE_DIR}/assets/images/dvd.png
)

mlk_executable(
	NAME example-states
	FOLDER examples
	LIBRARIES libmlk-example
	SOURCES ${ASSETS} ${SOURCES}
	ASSETS ${ASSETS}
)

source_group(TREE ${example-states_SOURCE_DIR} FILES ${ASSETS} ${SOURCES})
