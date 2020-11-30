#
# MolkoSetBuildDirectories.cmake -- CMake build system for molko
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
# # molko_set_build_directories
#
# Change build directories for the given target.
#
# ## Synopsis
#
# ```cmake
# molko_set_build_directories(target)
# ```
#
# This function will set output directories for the given target. It is
# necessary so that binaries can know where to find extra data.
#

function(_msbd_set_dir target property value)
	if (NOT IS_ABSOLUTE ${value})
		set_target_properties(
			${target}
			PROPERTIES
				${property} ${CMAKE_BINARY_DIR}/${value}
		)

		foreach (c ${CMAKE_CONFIGURATION_TYPES})
			string(TOUPPER ${c} cfg)
			set_target_properties(
				${target}
				PROPERTIES
					${property}_${cfg} ${CMAKE_BINARY_DIR}/${c}/${value}
			)
		endforeach ()
	endif ()

endfunction()

function(molko_set_build_directories target)
	_msbd_set_dir(${target} ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_INSTALL_LIBDIR})
	_msbd_set_dir(${target} LIBRARY_OUTPUT_DIRECTORY ${CMAKE_INSTALL_LIBDIR})
	_msbd_set_dir(${target} RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_BINDIR})
endfunction()
