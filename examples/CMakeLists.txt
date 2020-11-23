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

project(examples)

set(
	ASSETS_IMAGES
	${examples_SOURCE_DIR}/assets/images/battle-background.png
	${examples_SOURCE_DIR}/assets/images/black-cat.png
	${examples_SOURCE_DIR}/assets/images/haunted-wood.png
)

set(
	ASSETS_MAPS
	${examples_SOURCE_DIR}/assets/maps/map-rebonai.json
	${examples_SOURCE_DIR}/assets/maps/map-world.json
)

set(
	ASSETS_MAPS_SPRITES
	${examples_SOURCE_DIR}/assets/maps/sprite-town.png
	${examples_SOURCE_DIR}/assets/maps/sprite-world.png
)

set(
	ASSETS_MAPS_ANIMATIONS
	${examples_SOURCE_DIR}/assets/maps/animation-water.png
)

set(
	ASSETS_TILESETS
	${examples_SOURCE_DIR}/assets/maps/tileset-town.json
	${examples_SOURCE_DIR}/assets/maps/tileset-world.json
)

set(
	ASSETS_MUSIC
	${examples_SOURCE_DIR}/assets/music/vabsounds-romance.ogg
)

set(
	ASSETS_SOUNDS
	${examples_SOURCE_DIR}/assets/sounds/fire.wav
)

set(
	ASSETS_SPRITES
	${examples_SOURCE_DIR}/assets/sprites/chest.png
	${examples_SOURCE_DIR}/assets/sprites/cursor.png
	${examples_SOURCE_DIR}/assets/sprites/explosion.png
	${examples_SOURCE_DIR}/assets/sprites/john.png
	${examples_SOURCE_DIR}/assets/sprites/john-sword.png
	${examples_SOURCE_DIR}/assets/sprites/john-walk.png
	${examples_SOURCE_DIR}/assets/sprites/numbers.png
	${examples_SOURCE_DIR}/assets/sprites/people.png
)

set(
	ASSETS
	${ASSETS_IMAGES}
	${ASSETS_MUSIC}
	${ASSETS_SOUNDS}
	${ASSETS_SPRITES}
)

# Can't use an interface library as examples live in subdirectories.
file(WRITE ${examples_BINARY_DIR}/none.c "void molko() {}")

# These files just need to be copied.
file(MAKE_DIRECTORY ${cmake_BINARY_DIR}/assets/maps)

foreach (s ${ASSETS_MAPS_SPRITES} ${ASSETS_MAPS_ANIMATIONS})
	get_filename_component(basename ${s} NAME)
	set(output ${examples_BINARY_DIR}/assets/maps/${basename})

	add_custom_command(
		OUTPUT ${output}
		COMMENT "Copy ${basename}"
		DEPENDS ${s}
		COMMAND ${CMAKE_COMMAND} -E copy ${s} ${output}
		VERBATIM
	)

	list(APPEND COPY_OUTPUTS ${output})
endforeach ()

molko_define_library(
	TARGET libexamples
	FOLDER examples
	SOURCES
		${examples_BINARY_DIR}/none.c
		${COPY_OUTPUTS}
		${ASSETS}
		${ASSETS_MAPS}
		${ASSETS_TILESETS}
	PUBLIC_FLAGS
		BINDIR="${examples_BINARY_DIR}"
	ASSETS ${ASSETS}
	MAPS ${ASSETS_MAPS}
	TILESETS ${ASSETS_TILESETS}
)

add_subdirectory(example-action)
add_subdirectory(example-animation)
add_subdirectory(example-audio)
add_subdirectory(example-battle)
add_subdirectory(example-cursor)
add_subdirectory(example-debug)
add_subdirectory(example-drawable)
add_subdirectory(example-font)
add_subdirectory(example-gridmenu)
add_subdirectory(example-label)
add_subdirectory(example-map)
add_subdirectory(example-message)
add_subdirectory(example-sprite)
add_subdirectory(example-trace)
add_subdirectory(example-ui)

source_group("assets/images" FILES ${ASSETS_IMAGES})
source_group("assets/maps" FILES ${ASSETS_MAPS})
source_group("assets/music" FILES ${ASSETS_MUSIC})
source_group("assets/sounds" FILES ${ASSETS_SOUNDS})
source_group("assets/sprites" FILES ${ASSETS_SPRITES})
source_group("assets/tilesets" FILES ${ASSETS_TILESETS})
