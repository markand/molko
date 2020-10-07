#
# CMakeLists.txt -- CMake build system for libadventure
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

project(libadventure)

set(
	SOURCES
	${libadventure_SOURCE_DIR}/adventure/mainmenu_state.c
	${libadventure_SOURCE_DIR}/adventure/mainmenu_state.h
	${libadventure_SOURCE_DIR}/adventure/panic_state.c
	${libadventure_SOURCE_DIR}/adventure/panic_state.h
	${libadventure_SOURCE_DIR}/adventure/splashscreen_state.c
	${libadventure_SOURCE_DIR}/adventure/splashscreen_state.h
)

set(
	ASSETS
	${libadventure_SOURCE_DIR}/adventure/assets/maps/overworld.map
	${libadventure_SOURCE_DIR}/adventure/assets/tilesets/world.png
)

molko_define_library(
	TARGET libadventure
	SOURCES ${SOURCES}
	ASSETS ${ASSETS}
	LIBRARIES libcore
	PUBLIC_INCLUDES
		$<BUILD_INTERFACE:${libadventure_SOURCE_DIR}>
)

source_group(adventure FILES ${SOURCES})
