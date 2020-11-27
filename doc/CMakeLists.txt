#
# CMakeLists.txt -- CMake build system for molko
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

project(doc)

find_package(Doxygen QUIET)
find_program(MKDOCS_EXE mkdocs DOC "Path to mkdocs")
find_program(DOXYBOOK2_EXE doxybook2 DOC "Path to doxybook2")

if (DOXYGEN_FOUND AND MKDOCS_EXE AND DOXYBOOK2_EXE)
	set(
		DOXYGEN_SOURCES
		libcore
		libui
		librpg
		${doc_SOURCE_DIR}/doxygen/groups.c
	)

	set(
		DOXYGEN_STRIP_FROM_PATH
		${CMAKE_SOURCE_DIR}/libcore
		${CMAKE_SOURCE_DIR}/libui
		${CMAKE_SOURCE_DIR}/librpg
	)
	set(
		DOC_SOURCES
		${doc_SOURCE_DIR}/docs/specs/map.md
		${doc_SOURCE_DIR}/docs/specs/tileset.md
		${doc_SOURCE_DIR}/docs/tools/bcc.md
		${doc_SOURCE_DIR}/docs/tools/map.md
		${doc_SOURCE_DIR}/docs/tools/tileset.md
		${doc_SOURCE_DIR}/docs/about.md
		${doc_SOURCE_DIR}/docs/index.md
		${doc_SOURCE_DIR}/docs/install.md
		${doc_SOURCE_DIR}/mkdocs.yml
	)

	# 1. Generate XML structure with Doxygen.
	set(DOXYGEN_SHOW_GROUPED_MEMB_INC NO)
	set(DOXYGEN_SHOW_INCLUDE_FILES NO)
	set(DOXYGEN_SHOW_USED_FILES NO)
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
	set(DOXYGEN_XML_OUTPUT ${doc_BINARY_DIR}/doxygen-xml)
	set(DOXYGEN_QUIET YES)
	set(DOXYGEN_RECURSIVE YES)
	set(DOXYGEN_TAB_SIZE 8)
	set(DOXYGEN_WARNINGS YES)

	# 1. Generate XML from doxygen.
	doxygen_add_docs(
		doc-doxygen-xml
		${DOXYGEN_SOURCES}
		WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
	)

	# 2. Convert XML through markdown with doxybook2.
	add_custom_target(
		doc-doxybook
		VERBATIM
		DEPENDS doc-doxygen-xml
		COMMAND
			${CMAKE_COMMAND} -E make_directory ${doc_BINARY_DIR}/docs
		COMMAND ${DOXYBOOK2_EXE} -q
			-i ${doc_BINARY_DIR}/doxygen-xml
			-o ${doc_BINARY_DIR}/docs
			-c ${doc_SOURCE_DIR}/doxybook.json
	)

	# 3. Copy the directory containing hand-made documentation and call mkdocs.
	add_custom_target(
		doc
		VERBATIM
		DEPENDS doc-doxybook
		SOURCES ${DOC_SOURCES}
		WORKING_DIRECTORY ${doc_BINARY_DIR}
		COMMAND
			${CMAKE_COMMAND} -E copy_directory
				${doc_SOURCE_DIR}/docs
				${doc_BINARY_DIR}/docs
		COMMAND
			${CMAKE_COMMAND} -E copy
				${doc_SOURCE_DIR}/mkdocs.yml
				${doc_BINARY_DIR}
		COMMAND
			${MKDOCS_EXE} build
				-d ${doc_BINARY_DIR}/html
	)

	add_custom_target(
		doc-serve
		VERBATIM
		DEPENDS doc-doxybook
		SOURCES ${DOC_SOURCES}
		WORKING_DIRECTORY ${doc_BINARY_DIR}
		COMMAND
			${CMAKE_COMMAND} -E copy_directory
				${doc_SOURCE_DIR}/docs
				${doc_BINARY_DIR}/docs
		COMMAND
			${CMAKE_COMMAND} -E copy
				${doc_SOURCE_DIR}/mkdocs.yml
				${doc_BINARY_DIR}
		COMMAND
			${MKDOCS_EXE} serve
	)

	set_target_properties(doc doc-serve doc-doxygen-xml doc-doxybook
		PROPERTIES
			FOLDER doc
	)

	source_group(TREE ${doc_SOURCE_DIR}  FILES ${DOC_SOURCES})
endif ()
