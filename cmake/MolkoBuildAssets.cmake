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

#
# # molko_build_assets
#
# Generate custom commands to convert assets into C header files using mlk-bcc.
# The argument output will be set to contain every generated output so that the
# caller can set them as source input and be generated before the target itself.
#
# ## Synopsis:
#
# ```cmake
# molko_build_assets(assets outputs)
# ```
#
# ## Example
#
# ```cmake
# molko_build_assets(image.png outputs)
# add_executable(main main.c ${outputs})
# ```
#
# Do not forget to add the CMake current binary directory
# `${CMAKE_CURRENT_BINARY_DIR}` through the include flags of the given target.
#
# Each file is generated using the exact same file hierarchy as the input so
# an input of foo/bar/baz.png will be generated as foo/bar/baz.h in the binary
# directory. The exported symbol use the pattern <last-directory>_<basename> so
# in the above example, the file variable would be "bar_baz"
#

macro(molko_build_assets assets outputs)
	set(${outputs})

	foreach (a ${assets})
		file(RELATIVE_PATH basename ${CMAKE_CURRENT_SOURCE_DIR} ${a})
		string(REGEX REPLACE "\\..*$" ".h" output ${basename})
		set(output ${CMAKE_CURRENT_BINARY_DIR}/${output})
		get_filename_component(outputdir ${output} DIRECTORY)
		get_filename_component(name ${basename} NAME_WE)
		get_filename_component(category ${outputdir} NAME)
		get_filename_Component(extension ${a} EXT)
		file(MAKE_DIRECTORY ${outputdir})

		# For "text" files, we create a C array null-terminated.
		if (${extension} MATCHES "\\.(txt|sql)")
			set(arg0 -0)
		else ()
			unset(arg0)
		endif ()

		add_custom_command(
			OUTPUT ${output}
			COMMAND $<TARGET_FILE:mlk-bcc> ${arg0} -csu ${a} ${category}-${name} > ${output}
			DEPENDS
				${a}
				$<TARGET_FILE:mlk-bcc>
			COMMENT "Generate header file from ${basename}"
		)

		list(APPEND ${outputs} ${output})
	endforeach ()
endmacro()
