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

project(libmlk-util)

include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckSymbolExists)

set(
	SOURCES
	${libmlk-util_SOURCE_DIR}/mlk/util/fmemopen.c
	${libmlk-util_SOURCE_DIR}/mlk/util/openbsd/basename.c
	${libmlk-util_SOURCE_DIR}/mlk/util/openbsd/dirname.c
	${libmlk-util_SOURCE_DIR}/mlk/util/openbsd/getopt.c
	${libmlk-util_SOURCE_DIR}/mlk/util/openbsd/strlcat.c
	${libmlk-util_SOURCE_DIR}/mlk/util/openbsd/strlcpy.c
	${libmlk-util_SOURCE_DIR}/mlk/util/sysconfig.cmake.h
	${libmlk-util_SOURCE_DIR}/mlk/util/util.c
	${libmlk-util_SOURCE_DIR}/mlk/util/util.h
)

check_symbol_exists("PATH_MAX" "limits.h" MLK_HAVE_PATH_MAX)

check_include_file("libgen.h" MLK_HAVE_LIBGEN_H)

check_function_exists(basename MLK_HAVE_BASENAME)
check_function_exists(dirname MLK_HAVE_DIRNAME)
check_function_exists(fmemopen MLK_HAVE_FMEMOPEN)
check_function_exists(strlcat MLK_HAVE_STRLCAT)
check_function_exists(strlcpy MLK_HAVE_STRLCPY)

configure_file(
	${libmlk-util_SOURCE_DIR}/mlk/util/sysconfig.cmake.h
	${libmlk-util_BINARY_DIR}/sysconfig.h
)

mlk_library(
	NAME libmlk-util
	SOURCES ${SOURCES}
	TYPE STATIC
	LIBRARIES ${LIBRARIES}
	INCLUDES
		PUBLIC
			$<BUILD_INTERFACE:${libmlk-util_SOURCE_DIR}>
			$<BUILD_INTERFACE:${libmlk-util_BINARY_DIR}>
)

source_group(TREE ${libmlk-util_SOURCE_DIR} FILES ${SOURCES})
