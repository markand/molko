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

find_program(SPHINX_EXE sphinx-build DOC "Path to sphinx-build")

if (SPHINX_EXE)
	set(
		SOURCES
		${doc_SOURCE_DIR}/about.rst
		${doc_SOURCE_DIR}/conf.py
		${doc_SOURCE_DIR}/index.rst
		${doc_SOURCE_DIR}/spec-map.rst
		${doc_SOURCE_DIR}/spec-tileset.rst
		${doc_SOURCE_DIR}/specifications.rst
		${doc_SOURCE_DIR}/tool-bcc.rst
		${doc_SOURCE_DIR}/tool-map.rst
		${doc_SOURCE_DIR}/tool-tileset.rst
		${doc_SOURCE_DIR}/tools.rst
	)

	configure_file(
		${doc_SOURCE_DIR}/conf.py
		${doc_BINARY_DIR}/conf.py
	)

	add_custom_target(
		doc
		ALL VERBATIM
		SOURCES ${SOURCES}
		COMMAND ${SPHINX_EXE} -c ${doc_BINARY_DIR} -q . ${doc_BINARY_DIR}
		WORKING_DIRECTORY ${doc_SOURCE_DIR}
		COMMENT "Generating documentation"
		DEPENDS doxygen
	)
endif ()
