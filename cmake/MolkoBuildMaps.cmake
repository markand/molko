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
# Generate .map files from tiled .json files using the mlk-map tool. The file
# hierarchy is kept and only extension is changed from .json to .map.
#
# ## Synopsis
#
# ```cmake
# molko_build_maps(target input outputs)
# ```
#
# Argument outputs will be set with the generated output files in the binary
# directory to be added as target sources.
#
# Make sure to add `${CMAKE_CURRENT_BINARY_DIR}` into the target include
# directories.
#

macro(molko_build_maps target inputs outputs)
	set(${outputs})

	foreach (t ${inputs})
		file(RELATIVE_PATH basename ${CMAKE_CURRENT_SOURCE_DIR} ${t})
		string(REGEX REPLACE "\\.json$" ".map" output ${basename})
		if (NOT IS_ABSOLUTE ${CMAKE_INSTALL_DATADIR})
			set(output ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${CMAKE_INSTALL_DATADIR}/${target}/${output})
		else ()
			set(output ${CMAKE_CURRENT_BINARY_DIR}/${output})
		endif ()

		get_filename_component(outputdir ${output} DIRECTORY)

		add_custom_command(
			OUTPUT ${output}
			COMMAND
				${CMAKE_COMMAND} -E make_directory ${outputdir}
			COMMAND
				$<TARGET_FILE:mlk-map> < ${t} > ${output}
			COMMENT
				"Generating map from ${basename}"
			DEPENDS $<TARGET_FILE:mlk-map> ${t}
			VERBATIM
		)

		list(APPEND ${outputs} ${output})
	endforeach ()
endmacro ()
