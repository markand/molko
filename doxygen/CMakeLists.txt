#
# CMakeLists.txt -- CMake build system for doxygen
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

project(doxygen)

find_package(Doxygen QUIET)

if (DOXYGEN_FOUND)
	set(
		INPUT
		doxygen/groups.c
		doxygen/mainpage.c
		doxygen/page-examples.c
		doxygen/page-faq.c
		doxygen/page-howto-error.c
		doxygen/page-howto-initialization.c
		doxygen/page-howto-ownership.c
		doxygen/page-portability.c
		libadventure
		libcore
		libui
		librpg
	)

	set(
		DOXYGEN_STRIP_FROM_PATH
		${CMAKE_SOURCE_DIR}/libadventure
		${CMAKE_SOURCE_DIR}/libcore
		${CMAKE_SOURCE_DIR}/libui
		${CMAKE_SOURCE_DIR}/librpg
	)

	set(DOXYGEN_ALLOW_UNICODE_NAMES YES)
	set(DOXYGEN_AUTOLINK_SUPPORT NO)
	set(DOXYGEN_ENABLE_PREPROCESSING YES)
	set(DOXYGEN_EXAMPLE_PATH examples)
	set(DOXYGEN_EXCLUDE_PATTERNS *_p.h)
	set(DOXYGEN_FILE_PATTERNS *.h)
	set(DOXYGEN_GENERATE_LATEX NO)
	set(DOXYGEN_GENERATE_MAN NO)
	set(DOXYGEN_GENERATE_XML YES)
	set(DOXYGEN_GENERATE_HTML NO)
	set(DOXYGEN_INPUT_ENCODING UTF-8)
	set(DOXYGEN_MAX_INITIALIZER_LINES 0)
	set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
	set(DOXYGEN_OUTPUT_DIRECTORY doxygen)
	set(DOXYGEN_PREDEFINED DOXYGEN)
	set(DOXYGEN_PROJECT_BRIEF "2D RPG game in C")
	set(DOXYGEN_PROJECT_NAME "Molko's Adventure")
	set(DOXYGEN_QUIET YES)
	set(DOXYGEN_RECURSIVE YES)
	set(DOXYGEN_TAB_SIZE 8)
	set(DOXYGEN_WARNINGS YES)

	doxygen_add_docs(
		doxygen
		${INPUT}
		ALL
		WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
	)

	set_target_properties(doxygen PROPERTIES FOLDER misc)
endif ()
