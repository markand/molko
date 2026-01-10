#
# CMakeLists.txt -- CMake build system for Molko's Engine
#
# Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/loader-file_p.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/loader-file_p.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/map-loader-file.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/map-loader.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/map.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/message.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/property.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/quest.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/rpg.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/rpg_p.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/save.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/tileset-loader-file.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/tileset-loader.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/tileset.c
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/walksprite.c
)

set(
	HEADERS
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/map-loader-file.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/map-loader.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/map.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/message.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/property.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/quest.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/rpg.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/save.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/tileset-loader-file.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/tileset-loader.h
	${libmlk-rpg_SOURCE_DIR}/mlk/rpg/tileset.h
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
	SOURCES ${SOURCES}
	HEADERS ${HEADERS}
	HEADERS_DIRECTORY mlk/rpg
	LANGS ${NLS}
	ASSETS ${ASSETS}
	INSTALL
	LIBRARIES
		libmlk-ui
		libmlk-sqlite
		libmlk-utlist
	INCLUDES
		PRIVATE $<BUILD_INTERFACE:${libmlk-rpg_BINARY_DIR}>
		PUBLIC $<BUILD_INTERFACE:${libmlk-rpg_SOURCE_DIR}>
)

source_group(TREE ${libmlk-rpg_SOURCE_DIR} FILES ${SOURCES} ${HEADERS} ${ASSETS} ${NLS})
