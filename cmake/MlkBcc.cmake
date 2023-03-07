#
# CMakeLists.txt -- CMake build system for Molko's Engine
#
# Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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
# mlk_bcc(
#   OUTPUT_VAR variable
#   ASSETS files...
#   [OUTPUT_DIRECTORY directory]
# )
#
# Convert binary files to header files using mlk-bcc utility.
#
# This macro will generate all files under the OUTPUT_DIRECTORY variable which
# defaults to ${CMAKE_CURRENT_BINARY_DIR}/assets if unset.
#
#
# It will also keep the last parent directory name under the output directory
# because most users will arrange more files under a specific file hierarchy,
# this also avoid a file name conflict.
#
# Example, given the following assets files:
#
# - assets/sprites/foo.png
# - assets/sounds/volume-up.wav
# - assets/sounds/volume-down.wav
#
# The following files will be generated:
#
# - <output-directory>/assets/sprites/foo.h
# - <output-directory>/assets/sounds/volume-up.h
# - <output-directory>/assets/sounds/volume-down.h
#
macro(mlk_bcc)
	set(options "")
	set(oneValueArgs "OUTPUT_DIRECTORY;OUTPUTS_VAR")
	set(multiValueArgs ASSETS)

	cmake_parse_arguments(_bcc "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if (NOT _bcc_OUTPUTS_VAR)
		message(FATAL_ERROR "Missing OUTPUTS_VAR")
	elseif (NOT _bcc_ASSETS)
		message(FATAL_ERROR "Missing ASSETS")
	endif ()

	if (NOT _bcc_OUTPUT_DIRECTORY)
		set(_bcc_base_directory ${CMAKE_CURRENT_BINARY_DIR}/assets)
	else ()
		set(_bcc_base_directory ${_bcc_OUTPUT_DIRECTORY})
	endif ()

	foreach (a ${_bcc_ASSETS})
		# Determine output parent directory name.
		get_filename_component(_bcc_dirname ${a} DIRECTORY)
		get_filename_component(_bcc_dirname ${_bcc_dirname} NAME)

		# Determine output file name.
		cmake_path(GET a FILENAME _bcc_filename)
		cmake_path(REPLACE_EXTENSION _bcc_filename .h)
		set(_bcc_output_file ${_bcc_base_directory}/${_bcc_dirname}/${_bcc_filename})

		# Text files are better NUL terminated.
		cmake_path(GET a EXTENSION _bcc_extension)

		if (_bcc_extension MATCHES "\\.sql")
			set(_bcc_args "-0cs")
		else ()
			set(_bcc_args "-cs")
		endif ()

		# This is the mlk-bcc variable to generate the C identifier.
		set(_bcc_var "assets_${_bcc_dirname}_${_bcc_filename}")

		add_custom_command(
			OUTPUT ${_bcc_output_file}
			COMMAND
				${CMAKE_COMMAND} -E make_directory ${_bcc_base_directory}/${_bcc_dirname}
			COMMAND
				$<TARGET_FILE:mlk-bcc> ${_bcc_args} ${a} ${_bcc_var} > ${_bcc_output_file}
			COMMENT "Generating asset ${_bcc_dirname}/${_bcc_filename}"
			DEPENDS $<TARGET_FILE:mlk-bcc> ${a}
		)

		list(APPEND ${_bcc_OUTPUTS_VAR} ${_bcc_output_file})
	endforeach ()
endmacro()
