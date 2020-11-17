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

macro(molko_build_maps inputs outputs)
	set(${outputs})

	foreach (t ${inputs})
		file(RELATIVE_PATH basename ${CMAKE_CURRENT_SOURCE_DIR} ${t})
		string(REGEX REPLACE "\\.json$" ".map" output ${basename})
		set(output ${CMAKE_CURRENT_BINARY_DIR}/${output})
		get_filename_component(outputdir ${output} DIRECTORY)
		file(MAKE_DIRECTORY ${outputdir})

		add_custom_command(
			OUTPUT ${output}
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
