#
# CMakeLists.txt -- CMake build system for libcompat
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

cmake_minimum_required(VERSION 3.10)
project(libcompat)

include(CheckFunctionExists)
include(CheckIncludeFile)

set(
	SOURCES
	${libcompat_SOURCE_DIR}/src/compat.c
	${libcompat_SOURCE_DIR}/src/compat.h.in
)

set(
	FUNCTIONS
	# BSD extensions.
	pledge
	reallocarray
	recallocarray
	strdup
	strlcat
	strlcpy
	strndup
	strsep
	# POSIX functions.
	basename
	dirname
	getopt
	strnlen
)

set(
	INCLUDES
	# POSIX extensions.
	unistd.h
	libgen.h
)

foreach (f ${FUNCTIONS})
	string(TOUPPER ${f} var)

	check_function_exists(${f} COMPAT_HAVE_${var})

	if (NOT COMPAT_HAVE_${var})
		list(APPEND SOURCES ${libcompat_SOURCE_DIR}/src/${f}.c)
	endif ()
endforeach ()

configure_file(
	${libcompat_SOURCE_DIR}/src/compat.h.in
	${libcompat_BINARY_DIR}/compat.h
)

add_library(libcompat STATIC ${SOURCES})
target_include_directories(
	libcompat
	PUBLIC
		$<BUILD_INTERFACE:${libcompat_BINARY_DIR}>
)
target_compile_definitions(libcompat PUBLIC _BSD_SOURCE)
set_target_properties(libcompat PROPERTIES PREFIX "" FOLDER extern)

foreach (i ${INCLUDES})
	string(TOUPPER ${i} var)
	string(REGEX REPLACE "\\." "_" var ${var})

	check_include_file(${i} COMPAT_HAVE_${var})

	if (NOT COMPAT_HAVE_${var})
		file(WRITE ${libcompat_BINARY_DIR}/${i} "/* Empty stub for ${i}. */\n")
	endif ()
endforeach ()

# POSIX dirent.h
check_include_file(dirent.h COMPAT_HAVE_DIRENT_H)

if (NOT COMPAT_HAVE_DIRENT_H)
	if (CMAKE_SYSTEM_NAME MATCHES "Windows")
		target_include_directories(
			libcompat
			PUBLIC
				$<BUILD_INTERFACE:${libcompat_SOURCE_DIR}/win/dirent
		)
	endif ()
endif ()

# Math library
find_library(COMPAT_HAVE_LIBM m)

if (NOT COMPAT_HAVE_LIBM)
	add_library(m INTERFACE)
endif ()
