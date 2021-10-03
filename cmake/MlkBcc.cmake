#
# CMakeLists.txt -- CMake build system for Molko's Adventure
#
# Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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

macro(mlk_bcc)
	set(options "")
	set(oneValueArgs OUTPUTS_VAR)
	set(multiValueArgs ASSETS)

	cmake_parse_arguments(_bcc "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if (NOT _bcc_OUTPUTS_VAR)
		message(FATAL_ERROR "Missing OUTPUTS_VAR")
	elseif (NOT _bcc_ASSETS)
		message(FATAL_ERROR "Missing ASSETS")
	endif ()

	foreach (a ${_bcc_ASSETS})
		cmake_path(
			RELATIVE_PATH a
			BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			OUTPUT_VARIABLE output
		)
		cmake_path(GET output PARENT_PATH outputdir)
		cmake_path(GET output EXTENSION extension)
		cmake_path(REPLACE_EXTENSION output .h)
		cmake_path(REMOVE_EXTENSION output OUTPUT_VARIABLE outputname)

		if (extension MATCHES ".sql")
			set(args "-0csu")
		else ()
			set(args "-csu")
		endif ()

		set(outputfile ${CMAKE_CURRENT_BINARY_DIR}/${output})

		add_custom_command(
			OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${output}
			COMMAND
				${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${outputdir}
			COMMAND
				$<TARGET_FILE:mlk-bcc> ${args} ${a} ${outputname} > ${outputfile}
			COMMENT "Generating ${output}"
			DEPENDS $<TARGET_FILE:mlk-bcc> ${a}
		)

		list(APPEND ${_bcc_OUTPUTS_VAR} ${CMAKE_CURRENT_BINARY_DIR}/${output})
	endforeach ()
endmacro()
