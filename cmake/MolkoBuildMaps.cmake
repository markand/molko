#
# MolkoBuildMaps.cmake -- CMake build system for molko
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

#
# # molko_build_maps
#
# Convert [Tiled][] maps and tilesets into textual representations that can be
# loaded from Molko's Adventure API.
#
# ## Synopsis
#
# ```cmake
# molko_build_maps(
#   OUTPUT_DIR  output directory
#   OUTPUTS     output variables
#   MAPS        (Optional) List of maps
#   TILESETS    (Optional) List of tilesets
# )
# ```
#
# Argument MAPS and TILESETS should contain list of .json files generated from
# [Tiled][tiled] and store the result in OUTPUT_DIR
#
# Arguments OUTPUTS will be filled with genereted files from CMake and can be
# used as executable and input.
#
# [Tiled]: http://mapeditor.org
#

macro(molko_build_maps)
	set(options)
	set(oneValueArgs OUTPUTS OUTPUT_DIR)
	set(multiValueArgs MAPS TILESETS)

	cmake_parse_arguments(CV "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if (NOT CV_OUTPUT_DIR)
		message(FATAL_ERROR "Missing OUTPUT_DIR argument")
	endif ()
	if (NOT CV_OUTPUTS)
		message(FATAL_ERROR "Missing OUTPUTS argument")
	endif ()

	set(${CV_OUTPUTS})

	foreach (i ${CV_MAPS} ${CV_TILESETS})
		if (NOT IS_ABSOLUTE ${CMAKE_INSTALL_DATADIR})
			set(outputdir ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${CMAKE_INSTALL_DATADIR}/${CV_OUTPUT_DIR})
		else ()
			set(outputdir ${CMAKE_CURRENT_BINARY_DIR}/${CV_OUTPUT_DIR})
		endif ()

		if (${i} IN_LIST CV_MAPS)
			set(tool mlk-map)
			set(ext .map)
		else ()
			set(tool mlk-tileset)
			set(ext .tileset)
		endif ()

		string(REGEX REPLACE "\\.json$" ${ext} output ${i})
		get_filename_component(output ${output} NAME)

		add_custom_command(
			OUTPUT ${outputdir}/${output}
			VERBATIM
			COMMAND
				${CMAKE_COMMAND} -E make_directory ${outputdir}
			COMMAND
				$<TARGET_FILE:${tool}> < ${i} > ${outputdir}/${output}
			COMMENT
				"Generating object from ${i}"
			DEPENDS $<TARGET_FILE:${tool}> ${i}
		)

		list(APPEND ${CV_OUTPUTS} ${outputdir}/${output})

		install(
			FILES ${outputdir}/${output}
			DESTINATION ${CMAKE_INSTALL_DATADIR}/${CV_OUTPUT_DIR}
		)
	endforeach ()
endmacro ()
