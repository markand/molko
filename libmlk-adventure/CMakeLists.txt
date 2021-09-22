#
# CMakeLists.txt -- CMake build system for Molko's Adventure
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

project(libmlk-adventure)

set(
	SOURCES
	${libmlk-adventure_SOURCE_DIR}/adventure/action/chest.c
	${libmlk-adventure_SOURCE_DIR}/adventure/action/chest.h
	${libmlk-adventure_SOURCE_DIR}/adventure/action/spawner.c
	${libmlk-adventure_SOURCE_DIR}/adventure/action/spawner.h
	${libmlk-adventure_SOURCE_DIR}/adventure/action/teleport.c
	${libmlk-adventure_SOURCE_DIR}/adventure/action/teleport.h
	${libmlk-adventure_SOURCE_DIR}/adventure/adventure_p.h
	${libmlk-adventure_SOURCE_DIR}/adventure/assets.c
	${libmlk-adventure_SOURCE_DIR}/adventure/assets.h
	${libmlk-adventure_SOURCE_DIR}/adventure/character/black-cat.c
	${libmlk-adventure_SOURCE_DIR}/adventure/character/black-cat.h
	${libmlk-adventure_SOURCE_DIR}/adventure/character/neth.c
	${libmlk-adventure_SOURCE_DIR}/adventure/character/neth.h
	${libmlk-adventure_SOURCE_DIR}/adventure/dialog/save.c
	${libmlk-adventure_SOURCE_DIR}/adventure/dialog/save.h
	${libmlk-adventure_SOURCE_DIR}/adventure/item/potion.c
	${libmlk-adventure_SOURCE_DIR}/adventure/item/potion.h
	${libmlk-adventure_SOURCE_DIR}/adventure/mapscene/mapscene.c
	${libmlk-adventure_SOURCE_DIR}/adventure/mapscene/mapscene.h
	${libmlk-adventure_SOURCE_DIR}/adventure/molko.c
	${libmlk-adventure_SOURCE_DIR}/adventure/molko.h
	${libmlk-adventure_SOURCE_DIR}/adventure/spell/fire-minor.c
	${libmlk-adventure_SOURCE_DIR}/adventure/spell/fire-minor.h
	${libmlk-adventure_SOURCE_DIR}/adventure/state/battle.c
	${libmlk-adventure_SOURCE_DIR}/adventure/state/battle.h
	${libmlk-adventure_SOURCE_DIR}/adventure/state/continue.c
	${libmlk-adventure_SOURCE_DIR}/adventure/state/continue.h
	${libmlk-adventure_SOURCE_DIR}/adventure/state/mainmenu.c
	${libmlk-adventure_SOURCE_DIR}/adventure/state/mainmenu.h
	${libmlk-adventure_SOURCE_DIR}/adventure/state/map.c
	${libmlk-adventure_SOURCE_DIR}/adventure/state/map.h
	${libmlk-adventure_SOURCE_DIR}/adventure/state/panic.c
	${libmlk-adventure_SOURCE_DIR}/adventure/state/panic.h
	${libmlk-adventure_SOURCE_DIR}/adventure/state/splashscreen.c
	${libmlk-adventure_SOURCE_DIR}/adventure/state/splashscreen.h
	${libmlk-adventure_SOURCE_DIR}/adventure/trace_hud.c
	${libmlk-adventure_SOURCE_DIR}/adventure/trace_hud.h
)

set(
	NLS
	${libmlk-adventure_SOURCE_DIR}/nls/fr.po
)

mlk_library(
	NAME libmlk-adventure
	SOURCES ${SOURCES} ${NLS}
	LANGS ${NLS}
	LIBRARIES libmlk-rpg
	INCLUDES
		PUBLIC $<BUILD_INTERFACE:${libmlk-adventure_SOURCE_DIR}>
)

source_group(TREE ${libmlk-adventure_SOURCE_DIR} FILES ${SOURCES} ${NLS})
