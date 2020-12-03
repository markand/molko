#
# CMakeLists.txt -- CMake build system for libmlk-rpg
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

project(libmlk-rpg)

set(
	SOURCES
	${libmlk-rpg_SOURCE_DIR}/rpg/battle.c
	${libmlk-rpg_SOURCE_DIR}/rpg/battle.h
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-bar.c
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-bar.h
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-entity.c
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-entity.h
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-entity-state.c
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-entity-state.h
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-entity-state-attacking.c
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-entity-state-blinking.c
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-entity-state-moving.c
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-entity-state-normal.c
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-indicator.c
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-indicator.h
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-state.c
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-state.h
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-state-ai.c
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-state-attacking.c
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-state-check.c
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-state-closing.c
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-state-menu.c
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-state-lost.c
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-state-opening.c
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-state-selection.c
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-state-sub.c
	${libmlk-rpg_SOURCE_DIR}/rpg/battle-state-victory.c
	${libmlk-rpg_SOURCE_DIR}/rpg/character.c
	${libmlk-rpg_SOURCE_DIR}/rpg/character.h
	${libmlk-rpg_SOURCE_DIR}/rpg/equipment.c
	${libmlk-rpg_SOURCE_DIR}/rpg/equipment.h
	${libmlk-rpg_SOURCE_DIR}/rpg/item.c
	${libmlk-rpg_SOURCE_DIR}/rpg/item.h
	${libmlk-rpg_SOURCE_DIR}/rpg/map.c
	${libmlk-rpg_SOURCE_DIR}/rpg/map.h
	${libmlk-rpg_SOURCE_DIR}/rpg/map-file.c
	${libmlk-rpg_SOURCE_DIR}/rpg/map-file.h
	${libmlk-rpg_SOURCE_DIR}/rpg/message.c
	${libmlk-rpg_SOURCE_DIR}/rpg/message.h
	${libmlk-rpg_SOURCE_DIR}/rpg/rpg.c
	${libmlk-rpg_SOURCE_DIR}/rpg/rpg.h
	${libmlk-rpg_SOURCE_DIR}/rpg/rpg_p.h
	${libmlk-rpg_SOURCE_DIR}/rpg/selection.h
	${libmlk-rpg_SOURCE_DIR}/rpg/spell.c
	${libmlk-rpg_SOURCE_DIR}/rpg/spell.h
	${libmlk-rpg_SOURCE_DIR}/rpg/teleport.c
	${libmlk-rpg_SOURCE_DIR}/rpg/teleport.h
	${libmlk-rpg_SOURCE_DIR}/rpg/tileset.c
	${libmlk-rpg_SOURCE_DIR}/rpg/tileset.h
	${libmlk-rpg_SOURCE_DIR}/rpg/tileset-file.c
	${libmlk-rpg_SOURCE_DIR}/rpg/tileset-file.h
	${libmlk-rpg_SOURCE_DIR}/rpg/walksprite.c
	${libmlk-rpg_SOURCE_DIR}/rpg/walksprite.h
)

set(
	PO
	${libmlk-rpg_SOURCE_DIR}/nls/fr.po
)

molko_define_library(
	TARGET libmlk-rpg
	TRANSLATIONS fr
	SOURCES ${SOURCES} ${PO}
	LIBRARIES
		libmlk-core
		libmlk-ui
	PUBLIC_INCLUDES
		$<BUILD_INTERFACE:${libmlk-rpg_SOURCE_DIR}>
)

source_group(TREE ${libmlk-rpg_SOURCE_DIR} FILES ${SOURCES})
source_group(TREE ${libmlk-rpg_SOURCE_DIR} FILES ${PO})
