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

project(libmlk-rpg)

set(
	SOURCES
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-bar-default.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-bar-default.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-bar.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-bar.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-entity-state-attacking.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-entity-state-attacking.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-entity-state-blinking.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-entity-state-blinking.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-entity-state-moving.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-entity-state-moving.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-entity-state-normal.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-entity-state-normal.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-entity-state.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-entity-state.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-entity.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-entity.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-indicator.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-indicator.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-message.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-message.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state-ai.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state-ai.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state-attacking.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state-attacking.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state-check.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state-check.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state-closing.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state-closing.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state-item.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state-item.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state-lost.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state-lost.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state-menu.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state-menu.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state-opening.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state-opening.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state-rendering.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state-rendering.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state-selection.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state-selection.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state-victory.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state-victory.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle-state.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/battle.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/character.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/character.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/equipment.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/equipment.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/inventory.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/inventory.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/item.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/item.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/map-file.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/map-file.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/map.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/map.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/message.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/message.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/property.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/property.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/quest.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/quest.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/rpg.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/rpg.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/save.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/save.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/selection.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/selection.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/spell.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/spell.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/team.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/team.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/tileset-file.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/tileset-file.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/tileset.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/tileset.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/walksprite.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/walksprite.h
)

set(
	ASSETS
	${libmlk-rpg_SOURCE_DIR}/assets/sql/character-load.sql
	${libmlk-rpg_SOURCE_DIR}/assets/sql/character-save.sql
	${libmlk-rpg_SOURCE_DIR}/assets/sql/init.sql
	${libmlk-rpg_SOURCE_DIR}/assets/sql/property-load.sql
	${libmlk-rpg_SOURCE_DIR}/assets/sql/property-remove.sql
	${libmlk-rpg_SOURCE_DIR}/assets/sql/property-save.sql
	${libmlk-rpg_SOURCE_DIR}/assets/sql/quest-remove.sql
	${libmlk-rpg_SOURCE_DIR}/assets/sql/quest-save.sql
	${libmlk-rpg_SOURCE_DIR}/assets/sql/quest-step-load.sql
	${libmlk-rpg_SOURCE_DIR}/assets/sql/quest-step-save.sql
)

set(
	NLS
	${libmlk-rpg_SOURCE_DIR}/nls/fr.po
)

mlk_library(
	NAME libmlk-rpg
	SOURCES ${SOURCES} ${ASSETS} ${NLS}
	LANGS ${NLS}
	ASSETS ${ASSETS}
	LIBRARIES
		libmlk-ui
		libmlk-sqlite
	INCLUDES
		PRIVATE $<BUILD_INTERFACE:${libmlk-rpg_BINARY_DIR}>
		PUBLIC $<BUILD_INTERFACE:${libmlk-rpg_SOURCE_DIR}>
)

source_group(TREE ${libmlk-rpg_SOURCE_DIR} FILES ${SOURCES} ${ASSETS} ${NLS})
