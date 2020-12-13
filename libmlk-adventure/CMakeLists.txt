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
	${libadventure_SOURCE_DIR}/adventure/action/chest.c
	${libadventure_SOURCE_DIR}/adventure/action/chest.h
	${libadventure_SOURCE_DIR}/adventure/action/spawner.c
	${libadventure_SOURCE_DIR}/adventure/action/spawner.h
	${libadventure_SOURCE_DIR}/adventure/action/teleport.c
	${libadventure_SOURCE_DIR}/adventure/action/teleport.h
	${libadventure_SOURCE_DIR}/adventure/adventure_p.h
	${libadventure_SOURCE_DIR}/adventure/assets.c
	${libadventure_SOURCE_DIR}/adventure/assets.h
	${libadventure_SOURCE_DIR}/adventure/mapscene/mapscene.c
	${libadventure_SOURCE_DIR}/adventure/mapscene/mapscene.h
	${libadventure_SOURCE_DIR}/adventure/molko.c
	${libadventure_SOURCE_DIR}/adventure/molko.h
	${libadventure_SOURCE_DIR}/adventure/state/mainmenu.c
	${libadventure_SOURCE_DIR}/adventure/state/mainmenu.h
	${libadventure_SOURCE_DIR}/adventure/state/map.c
	${libadventure_SOURCE_DIR}/adventure/state/map.h
	${libadventure_SOURCE_DIR}/adventure/state/panic.c
	${libadventure_SOURCE_DIR}/adventure/state/panic.h
	${libadventure_SOURCE_DIR}/adventure/state/splashscreen.c
	${libadventure_SOURCE_DIR}/adventure/state/splashscreen.h
	${libadventure_SOURCE_DIR}/adventure/trace_hud.c
	${libadventure_SOURCE_DIR}/adventure/trace_hud.h
)

set(
	PO
	${libadventure_SOURCE_DIR}/nls/fr.po
)

molko_define_library(
	TARGET libmlk-adventure
	TRANSLATIONS fr
	SOURCES ${SOURCES} ${PO}
	FLAGS PRIVATE _BSD_SOURCE
	LIBRARIES PUBLIC libmlk-core libmlk-ui libmlk-rpg libmlk-data
	INCLUDES PUBLIC $<BUILD_INTERFACE:${libadventure_SOURCE_DIR}>
)

source_group(TREE ${libadventure_SOURCE_DIR} FILES ${SOURCES})
source_group(TREE ${libadventure_SOURCE_DIR} FILES ${ASSETS})
source_group(TREE ${libadventure_SOURCE_DIR} FILES ${PO})
