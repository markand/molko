#
# CMakeLists.txt -- CMake build system for Molko's Adventure
#
# Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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

project(libmlk-data)

set(
	MAPS
	${libmlk-data_SOURCE_DIR}/maps/map-world.json
)

set(
	TILESETS
	${libmlk-data_SOURCE_DIR}/maps/tileset-world.json
)

foreach (m ${MAPS})
	cmake_path(GET m STEM name)
	mlk_map(${m} ${libmlk-data_SOURCE_DIR}/maps/${name}.map)
	list(APPEND maps ${libmlk-data_SOURCE_DIR}/maps/${name}.map)
endforeach ()

foreach (t ${TILESETS})
	cmake_path(GET t STEM name)
	mlk_tileset(${t} ${libmlk-data_SOURCE_DIR}/maps/${name}.tileset)
	list(APPEND tilesets ${libmlk-data_SOURCE_DIR}/maps/${name}.tileset)
endforeach ()

set(
	SOURCES
	${libmlk-data_SOURCE_DIR}/fonts/cubic.ttf
	${libmlk-data_SOURCE_DIR}/fonts/lato.ttf
	${libmlk-data_SOURCE_DIR}/fonts/pirata-one.ttf
	${libmlk-data_SOURCE_DIR}/fonts/teutonic.ttf
	${libmlk-data_SOURCE_DIR}/images/battle-background.png
	${libmlk-data_SOURCE_DIR}/images/black-cat.png
	${libmlk-data_SOURCE_DIR}/images/haunted-wood.png
	${libmlk-data_SOURCE_DIR}/maps/animation-water.png
	${libmlk-data_SOURCE_DIR}/maps/sprite-world.png
	${libmlk-data_SOURCE_DIR}/music/vabsounds-romance.ogg
	${libmlk-data_SOURCE_DIR}/sounds/fire.wav
	${libmlk-data_SOURCE_DIR}/sounds/potion.wav
	${libmlk-data_SOURCE_DIR}/sprites/chest.png
	${libmlk-data_SOURCE_DIR}/sprites/chest.xcf
	${libmlk-data_SOURCE_DIR}/sprites/explosion.png
	${libmlk-data_SOURCE_DIR}/sprites/faces.png
	${libmlk-data_SOURCE_DIR}/sprites/john-sword.png
	${libmlk-data_SOURCE_DIR}/sprites/john-sword.xcf
	${libmlk-data_SOURCE_DIR}/sprites/john-walk.png
	${libmlk-data_SOURCE_DIR}/sprites/john-walk.xcf
	${libmlk-data_SOURCE_DIR}/sprites/john.png
	${libmlk-data_SOURCE_DIR}/sprites/numbers.png
	${libmlk-data_SOURCE_DIR}/sprites/people.png
	${libmlk-data_SOURCE_DIR}/sprites/ui-cursor.png
)

add_custom_target(
	libmlk-data
	ALL
	SOURCES ${SOURCES} ${maps} ${tilesets}
)

foreach (dir fonts images maps music sounds sprites)
	install(
		DIRECTORY ${libmlk-data_SOURCE_DIR}/${dir}/
		DESTINATION ${CMAKE_INSTALL_DATADIR}/mlk-adventure/${dir}
		FILES_MATCHING REGEX "[.]png$|[.]ttf$|[.]wav$|[.]ogg$"
	)
endforeach ()

# Generated maps and tilesets.
install(
	FILES
		${maps}
		${tilesets}
	DESTINATION ${CMAKE_INSTALL_DATADIR}/mlk-adventure/maps
)

source_group(TREE ${libmlk-data_SOURCE_DIR} FILES ${SOURCES})
