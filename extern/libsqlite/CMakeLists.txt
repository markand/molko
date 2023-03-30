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

project(libmlk-sqlite)

set(
	SOURCES
	${libmlk-sqlite_SOURCE_DIR}/sqlite3.c
	${libmlk-sqlite_SOURCE_DIR}/sqlite3.h
)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
	list(APPEND OPTIONS -Wno-unused-parameter)
endif ()

mlk_library(
	NAME libmlk-sqlite
	SOURCES ${SOURCES}
	FOLDER extern
	OPTIONS PRIVATE ${OPTIONS}
	INSTALL
	FLAGS
		PRIVATE
			SQLITE_THREADSAFE=0
			SQLITE_DEFAULT_MEMSTATUS=0
			SQLITE_OMIT_DECLTYPE
			SQLITE_OMIT_DEPRECATED
			SQLITE_OMIT_LOAD_EXTENSION
			SQLITE_DEFAULT_FOREIGN_KEYS=1
	INCLUDES
		PUBLIC $<BUILD_INTERFACE:${libmlk-sqlite_SOURCE_DIR}>
)

source_group(TREE ${libmlk-sqlite_SOURCE_DIR} FILES ${SOURCES})
