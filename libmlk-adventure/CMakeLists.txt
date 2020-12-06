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
	${libadventure_SOURCE_DIR}/adventure/actions/teleport.c
	${libadventure_SOURCE_DIR}/adventure/actions/teleport.h
	${libadventure_SOURCE_DIR}/adventure/adventure_p.h
	${libadventure_SOURCE_DIR}/adventure/molko.c
	${libadventure_SOURCE_DIR}/adventure/molko.h
	${libadventure_SOURCE_DIR}/adventure/trace_hud.c
	${libadventure_SOURCE_DIR}/adventure/trace_hud.h
	${libadventure_SOURCE_DIR}/adventure/state/mainmenu.c
	${libadventure_SOURCE_DIR}/adventure/state/mainmenu.h
	${libadventure_SOURCE_DIR}/adventure/state/map.c
	${libadventure_SOURCE_DIR}/adventure/state/map.h
	${libadventure_SOURCE_DIR}/adventure/state/panic.c
	${libadventure_SOURCE_DIR}/adventure/state/panic.h
	${libadventure_SOURCE_DIR}/adventure/state/splashscreen.c
	${libadventure_SOURCE_DIR}/adventure/state/splashscreen.h
)

set(
	ASSETS
	${libadventure_SOURCE_DIR}/assets/fonts/cubic.ttf
	${libadventure_SOURCE_DIR}/assets/fonts/lato.ttf
	${libadventure_SOURCE_DIR}/assets/fonts/teutonic.ttf
	${libadventure_SOURCE_DIR}/assets/fonts/pirata-one.ttf
	${libadventure_SOURCE_DIR}/assets/sprites/john.png
)

set(
	MAPS
	${libadventure_SOURCE_DIR}/assets/maps/map-world.json
)

set(
	TILESETS
	${libadventure_SOURCE_DIR}/assets/maps/tileset-world.json
)

set(
	DATA
	${libadventure_SOURCE_DIR}/assets/maps/animation-water.png
	${libadventure_SOURCE_DIR}/assets/maps/sprite-world.png
	${libadventure_SOURCE_DIR}/assets/sprites/john.png
)

set(
	PO
	${libadventure_SOURCE_DIR}/nls/fr.po
)

molko_define_library(
	TARGET libmlk-adventure
	TRANSLATIONS fr
	SOURCES ${SOURCES} ${PO} ${ASSETS} ${MAPS} ${TILESETS}
	MAPS ${MAPS}
	TILESETS ${TILESETS}
	ASSETS ${ASSETS}
	DATA ${DATA}
	LIBRARIES libmlk-core libmlk-ui libmlk-rpg
	PRIVATE_INCLUDES
		${libadventure_SOURCE_DIR}/adventure
	PUBLIC_INCLUDES
		$<BUILD_INTERFACE:${libadventure_SOURCE_DIR}>
	PRIVATE_FLAGS DIRECTORY="${libadventure_SOURCE_DIR}/adventure/assets"
)

source_group(TREE ${libadventure_SOURCE_DIR} FILES ${SOURCES})
source_group(TREE ${libadventure_SOURCE_DIR} FILES ${ASSETS})
source_group(TREE ${libadventure_SOURCE_DIR} FILES ${PO})
