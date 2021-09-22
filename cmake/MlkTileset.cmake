#
# CMakeLists.txt -- CMake build system for Molko's Adventure
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

macro(mlk_tileset)
	set(options "")
	set(oneValueArgs "OUTPUTS_VAR")
	set(multiValueArgs "TILESETS")

	cmake_parse_arguments(_tileset "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if (NOT _tileset_OUTPUTS_VAR)
		message(FATAL_ERROR "Missing OUTPUTS_VAR")
	elseif (NOT _tileset_TILESETS)
		message(FATAL_ERROR "Missing TILESETS")
	endif ()

	foreach (m ${_tileset_TILESETS})
		cmake_path(GET m FILENAME name)
		cmake_path(REMOVE_EXTENSION name)

		if (MLK_WITH_ZSTD)
			set(_tileset_cmds
				COMMAND $<TARGET_FILE:mlk-tileset> < ${m} > ${CMAKE_CURRENT_BINARY_DIR}/${name}.tmp
				COMMAND
					ZSTD::exe -17 -fq
						--rm ${CMAKE_CURRENT_BINARY_DIR}/${name}.tmp
						-o ${CMAKE_CURRENT_BINARY_DIR}/${name}.tileset
			)
		else ()
			set(_tileset_cmds
				COMMAND $<TARGET_FILE:mlk-tileset> < ${m} > ${CMAKE_CURRENT_BINARY_DIR}/${name}.tileset
			)
		endif ()

		add_custom_command(
			OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${name}.tileset
			COMMAND
				${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}
			COMMAND
				${_tileset_cmds}
			DEPENDS $<TARGET_FILE:mlk-tileset>
			COMMENT "Generating ${name}.tileset"
		)

		list(APPEND ${_tileset_OUTPUTS_VAR} ${CMAKE_CURRENT_BINARY_DIR}/${name}.tileset)
	endforeach ()
endmacro()
