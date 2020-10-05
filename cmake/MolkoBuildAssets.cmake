#
# MolkoBuildAssets.cmake -- CMake build system for molko
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

macro(molko_build_assets assets outputs)
	set(${outputs})

	foreach (a ${assets})
		file(RELATIVE_PATH basename ${CMAKE_CURRENT_SOURCE_DIR} ${a})
		string(REGEX REPLACE "\\..*$" ".h" output ${basename})
		set(output ${CMAKE_CURRENT_BINARY_DIR}/${output})
		get_filename_component(outputdir ${output} DIRECTORY)
		get_filename_component(name ${basename} NAME_WE)

		file(MAKE_DIRECTORY ${outputdir})

		add_custom_command(
			OUTPUT ${output}
			COMMAND $<TARGET_FILE:bcc> -s ${a} ${name} > ${output}
			DEPENDS $<TARGET_FILE:bcc>
			COMMENT "Generate header file for ${basename}"
		)

		list(APPEND ${outputs} ${output})
	endforeach ()
endmacro()