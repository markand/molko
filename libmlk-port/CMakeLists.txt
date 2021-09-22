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

project(libmlk-port)

include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckSymbolExists)
include(CheckTypeSize)

check_function_exists(strlcpy MLK_HAS_STRLCPY)
check_function_exists(fmemopen MLK_HAS_FMEMOPEN)
check_function_exists(basename MLK_HAS_BASENAME)
check_function_exists(dirname MLK_HAS_DIRNAME)
check_function_exists(getopt MLK_HAS_GETOPT)
check_include_file(stdnoreturn.h MLK_HAS_STDNORETURN_H)
check_include_file(libgen.h MLK_HAS_LIBGEN_H)
check_include_file(unistd.h MLK_HAS_UNISTD_H)
check_symbol_exists(PATH_MAX limits.h MLK_HAS_PATH_MAX)
check_type_size(ssize_t MLK_HAS_SSIZE_T)

if (NOT MLK_HAS_STDNORETURN_H)
	file(WRITE ${CMAKE_BINARY_DIR}/stdnoreturn.h "#define noreturn\n")
endif ()
if (NOT MLK_HAS_LIBGEN_H)
	file(TOUCH ${CMAKE_BINARY_DIR}/libgen.h)
endif ()
if (NOT MLK_HAS_UNISTD_H)
	file(TOUCH ${CMAKE_BINARY_DIR}/unistd.h)
endif ()

configure_file(
	${libmlk-port_SOURCE_DIR}/port/config.h.in
	${libmlk-port_BINARY_DIR}/config.h
)

set(
	SOURCES
	${libmlk-port_SOURCE_DIR}/port/port.c
	${libmlk-port_SOURCE_DIR}/port/port.h
)

mlk_library(
	NAME libmlk-port
	SOURCES ${SOURCES}
	TYPE STATIC
	INCLUDES
		PUBLIC
			$<BUILD_INTERFACE:${libmlk-port_SOURCE_DIR}>
			$<BUILD_INTERFACE:${libmlk-port_BINARY_DIR}>
)

source_group(TREE ${libmlk-port_SOURCE_DIR} FILES ${SOURCES})
