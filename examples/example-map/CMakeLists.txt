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

project(example-map)

# TODO: replace this once we have a macro to build map.
add_custom_command(
	OUTPUT
		${example-map_BINARY_DIR}/simple.map
		${example-map_BINARY_DIR}/simple.png
	DEPENDS
		$<TARGET_FILE:map>
		${examples_SOURCE_DIR}/assets/maps/simple.json
		${examples_SOURCE_DIR}/assets/maps/simple.png
	COMMAND
		$<TARGET_FILE:map>
			< ${examples_SOURCE_DIR}/assets/maps/simple.json
			> ${example-map_BINARY_DIR}/simple.map
	COMMAND
		${CMAKE_COMMAND} -E copy
			${examples_SOURCE_DIR}/assets/maps/simple.png
			${example-map_BINARY_DIR}/simple.png
)

molko_define_executable(
	TARGET example-map
	SOURCES
		${example-map_SOURCE_DIR}/main.c
		${example-map_BINARY_DIR}/simple.map
		${example-map_BINARY_DIR}/simple.png
	FOLDER examples
	LIBRARIES librpg libexamples
	FLAGS BINDIR="${example-map_BINARY_DIR}"
)
