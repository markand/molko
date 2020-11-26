#
# CMakeLists.txt -- CMake build system for librpg
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

project(librpg)

set(
	SOURCES
	${librpg_SOURCE_DIR}/rpg/battle.c
	${librpg_SOURCE_DIR}/rpg/battle.h
	${librpg_SOURCE_DIR}/rpg/battle-bar.c
	${librpg_SOURCE_DIR}/rpg/battle-bar.h
	${librpg_SOURCE_DIR}/rpg/battle-entity.c
	${librpg_SOURCE_DIR}/rpg/battle-entity.h
	${librpg_SOURCE_DIR}/rpg/battle-entity-state.c
	${librpg_SOURCE_DIR}/rpg/battle-entity-state.h
	${librpg_SOURCE_DIR}/rpg/battle-entity-state-attacking.c
	${librpg_SOURCE_DIR}/rpg/battle-entity-state-attacking.h
	${librpg_SOURCE_DIR}/rpg/battle-entity-state-blinking.c
	${librpg_SOURCE_DIR}/rpg/battle-entity-state-blinking.h
	${librpg_SOURCE_DIR}/rpg/battle-entity-state-moving.c
	${librpg_SOURCE_DIR}/rpg/battle-entity-state-moving.h
	${librpg_SOURCE_DIR}/rpg/battle-entity-state-normal.c
	${librpg_SOURCE_DIR}/rpg/battle-entity-state-normal.h
	${librpg_SOURCE_DIR}/rpg/battle-indicator.c
	${librpg_SOURCE_DIR}/rpg/battle-indicator.h
	${librpg_SOURCE_DIR}/rpg/battle-state.c
	${librpg_SOURCE_DIR}/rpg/battle-state.h
	${librpg_SOURCE_DIR}/rpg/battle-state-ai.c
	${librpg_SOURCE_DIR}/rpg/battle-state-ai.h
	${librpg_SOURCE_DIR}/rpg/battle-state-attacking.c
	${librpg_SOURCE_DIR}/rpg/battle-state-attacking.h
	${librpg_SOURCE_DIR}/rpg/battle-state-check.c
	${librpg_SOURCE_DIR}/rpg/battle-state-check.h
	${librpg_SOURCE_DIR}/rpg/battle-state-closing.c
	${librpg_SOURCE_DIR}/rpg/battle-state-closing.h
	${librpg_SOURCE_DIR}/rpg/battle-state-menu.c
	${librpg_SOURCE_DIR}/rpg/battle-state-menu.h
	${librpg_SOURCE_DIR}/rpg/battle-state-lost.c
	${librpg_SOURCE_DIR}/rpg/battle-state-lost.h
	${librpg_SOURCE_DIR}/rpg/battle-state-opening.c
	${librpg_SOURCE_DIR}/rpg/battle-state-opening.h
	${librpg_SOURCE_DIR}/rpg/battle-state-selection.c
	${librpg_SOURCE_DIR}/rpg/battle-state-selection.h
	${librpg_SOURCE_DIR}/rpg/battle-state-sub.c
	${librpg_SOURCE_DIR}/rpg/battle-state-sub.h
	${librpg_SOURCE_DIR}/rpg/battle-state-victory.c
	${librpg_SOURCE_DIR}/rpg/battle-state-victory.h
	${librpg_SOURCE_DIR}/rpg/character.c
	${librpg_SOURCE_DIR}/rpg/character.h
	${librpg_SOURCE_DIR}/rpg/equipment.c
	${librpg_SOURCE_DIR}/rpg/equipment.h
	${librpg_SOURCE_DIR}/rpg/item.c
	${librpg_SOURCE_DIR}/rpg/item.h
	${librpg_SOURCE_DIR}/rpg/map.c
	${librpg_SOURCE_DIR}/rpg/map.h
	${librpg_SOURCE_DIR}/rpg/map-file.c
	${librpg_SOURCE_DIR}/rpg/map-file.h
	${librpg_SOURCE_DIR}/rpg/message.c
	${librpg_SOURCE_DIR}/rpg/message.h
	${librpg_SOURCE_DIR}/rpg/rpg.c
	${librpg_SOURCE_DIR}/rpg/rpg.h
	${librpg_SOURCE_DIR}/rpg/selection.h
	${librpg_SOURCE_DIR}/rpg/spell.c
	${librpg_SOURCE_DIR}/rpg/spell.h
	${librpg_SOURCE_DIR}/rpg/teleport.c
	${librpg_SOURCE_DIR}/rpg/teleport.h
	${librpg_SOURCE_DIR}/rpg/tileset.c
	${librpg_SOURCE_DIR}/rpg/tileset.h
	${librpg_SOURCE_DIR}/rpg/tileset-file.c
	${librpg_SOURCE_DIR}/rpg/tileset-file.h
	${librpg_SOURCE_DIR}/rpg/walksprite.c
	${librpg_SOURCE_DIR}/rpg/walksprite.h
)

molko_define_library(
	TARGET librpg
	SOURCES ${SOURCES}
	LIBRARIES
		libcore
		libui
	PUBLIC_INCLUDES
		$<BUILD_INTERFACE:${librpg_SOURCE_DIR}>
)

source_group(rpg FILES ${SOURCES})

