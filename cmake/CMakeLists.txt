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

project(cmake)

add_custom_target(
	cmake
	SOURCES
		${cmake_SOURCE_DIR}/FindJansson.cmake
		${cmake_SOURCE_DIR}/FindSDL2.cmake
		${cmake_SOURCE_DIR}/MolkoBuildAssets.cmake
		${cmake_SOURCE_DIR}/MolkoDefineExecutable.cmake
		${cmake_SOURCE_DIR}/MolkoDefineLibrary.cmake
		${cmake_SOURCE_DIR}/MolkoDefineTest.cmake
	COMMENT
		"Dummy target to list CMake files in IDEs"
)

set_target_properties(cmake PROPERTIES FOLDER misc)
