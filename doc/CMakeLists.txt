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
 		${doc_SOURCE_DIR}/api-js.rst
 		${doc_SOURCE_DIR}/api-molko-clock.rst
 		${doc_SOURCE_DIR}/api-molko-event.rst
 		${doc_SOURCE_DIR}/api-molko-font.rst
 		${doc_SOURCE_DIR}/api-molko-painter.rst
 		${doc_SOURCE_DIR}/api-molko-sprite.rst
 		${doc_SOURCE_DIR}/api-molko-texture.rst
 		${doc_SOURCE_DIR}/api-molko-util.rst
 		${doc_SOURCE_DIR}/api-molko-window.rst
 		${doc_SOURCE_DIR}/conf.py
 		${doc_SOURCE_DIR}/getting-started.rst
 		${doc_SOURCE_DIR}/index.rst
	)

	add_custom_target(
		doc
		ALL VERBATIM
		SOURCES ${SOURCES}
		COMMAND ${SPHINX_EXE} -q . ${doc_BINARY_DIR}
		WORKING_DIRECTORY ${doc_SOURCE_DIR}
		COMMENT "Generating documentation"
	)
endif ()
