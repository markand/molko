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

project(libmlk-example)

set(
	SOURCES
	${libmlk-example_SOURCE_DIR}/mlk/example/example.c
	${libmlk-example_SOURCE_DIR}/mlk/example/example.h
	${libmlk-example_SOURCE_DIR}/mlk/example/glower.c
	${libmlk-example_SOURCE_DIR}/mlk/example/glower.h
	${libmlk-example_SOURCE_DIR}/mlk/example/registry.c
	${libmlk-example_SOURCE_DIR}/mlk/example/registry.h
	${libmlk-example_SOURCE_DIR}/mlk/example/trace-hud.c
	${libmlk-example_SOURCE_DIR}/mlk/example/trace-hud.h
)

set(
	ASSETS
	${libmlk-example_SOURCE_DIR}/assets/images/battle-background.png
	${libmlk-example_SOURCE_DIR}/assets/images/black-cat.png
	${libmlk-example_SOURCE_DIR}/assets/images/haunted-wood.png
	${libmlk-example_SOURCE_DIR}/assets/images/sword.png
	${libmlk-example_SOURCE_DIR}/assets/music/vabsounds-romance.ogg
	${libmlk-example_SOURCE_DIR}/assets/sounds/fire.wav
	${libmlk-example_SOURCE_DIR}/assets/sounds/open-chest.wav
	${libmlk-example_SOURCE_DIR}/assets/sprites/chest.png
	${libmlk-example_SOURCE_DIR}/assets/sprites/explosion.png
	${libmlk-example_SOURCE_DIR}/assets/sprites/john-sword.png
	${libmlk-example_SOURCE_DIR}/assets/sprites/john-walk.png
	${libmlk-example_SOURCE_DIR}/assets/sprites/numbers.png
	${libmlk-example_SOURCE_DIR}/assets/sprites/people.png
	${libmlk-example_SOURCE_DIR}/assets/sprites/ui-cursor.png
)

set(
	MAPS
	${libmlk-example_SOURCE_DIR}/assets/maps/world.json
)

set(
	TILESETS
	${libmlk-example_SOURCE_DIR}/assets/tilesets/world.json
)

mlk_library(
	NAME libmlk-example
	SOURCES ${SOURCES}
	ASSETS ${ASSETS}
	TYPE STATIC
	LIBRARIES libmlk-rpg
	MAPS ${MAPS}
	TILESETS ${TILESETS}
	INCLUDES
		PUBLIC
			$<BUILD_INTERFACE:${libmlk-example_SOURCE_DIR}>
			$<BUILD_INTERFACE:${libmlk-example_BINARY_DIR}>
)

source_group(TREE ${libmlk-example_SOURCE_DIR} FILES ${ASSETS} ${SOURCES} ${MAPS} ${TILESETS})
