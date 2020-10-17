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
	${librpg_SOURCE_DIR}/rpg/inventory.c
	${librpg_SOURCE_DIR}/rpg/inventory.h
	${librpg_SOURCE_DIR}/rpg/item.h
	${librpg_SOURCE_DIR}/rpg/map.c
	${librpg_SOURCE_DIR}/rpg/map.h
	${librpg_SOURCE_DIR}/rpg/map_state.c
	${librpg_SOURCE_DIR}/rpg/map_state.h
	${librpg_SOURCE_DIR}/rpg/message.c
	${librpg_SOURCE_DIR}/rpg/message.h
	${librpg_SOURCE_DIR}/rpg/rpg.c
	${librpg_SOURCE_DIR}/rpg/rpg.h
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

