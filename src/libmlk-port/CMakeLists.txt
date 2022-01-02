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

project(libmlk-port)

set(
	SOURCES
	${libmlk-port_SOURCE_DIR}/port/basename.c
	${libmlk-port_SOURCE_DIR}/port/dirname.c
	${libmlk-port_SOURCE_DIR}/port/fmemopen.c
	${libmlk-port_SOURCE_DIR}/port/getopt.c
	${libmlk-port_SOURCE_DIR}/port/strlcat.c
	${libmlk-port_SOURCE_DIR}/port/strlcpy.c
)

mlk_library(
	NAME libmlk-port
	SOURCES ${SOURCES}
	TYPE STATIC
	LIBRARIES ${LIBRARIES}
	INCLUDES
		PUBLIC
			$<BUILD_INTERFACE:${libmlk-port_SOURCE_DIR}>
			$<BUILD_INTERFACE:${libmlk-port_BINARY_DIR}>
)

source_group(TREE ${libmlk-port_SOURCE_DIR} FILES ${SOURCES})
