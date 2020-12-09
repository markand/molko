#
# MolkoDefineLibrary.cmake -- CMake build system for molko
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
# # molko_define_library
#
# Create any kind of library.
#
# ## Synopsis
#
# ```cmake
# molko_define_library(
#   TARGET              target name
#   SOURCES             src1, src2, srcn
#   EXTERNAL            (Optional) set to true for external libraries
#   FOLDER              (Optional) optional subfolder to organize
#   TYPE                (Optional) type of library
#   LIBRARIES           (Optional) libraries to link
#   FLAGS               (Optional) C flags (without -D)
#   INCLUDES            (Optional) local includes for the target only
# )
# ```
#
# Create a library and optionally install it.
#
# The function create a new library named with the parameter *TARGET*, you
# should prefix it with "lib" as its the convention within molko (e.g. libfoo),
# the prefix is automatically removed.
#
# The argument *SOURCES* should contains the C source files and *HEADERS*
# should points to a directory to be installed verbatim in the include
# directory.
#
# Optional argument *EXTERNAL* should be set for targets that are not
# maintained here (e.g. third party libraries embedded).
#
# The optional arguments *LIBRARIES*, *FLAGS* and *INCLUDES* are passed to the
# respective CMake command [target_link_libraries][],
# [target_compile_definitions][] and [target_include_directories][]
# respectively. As such, it is necessary to specify the scope (PUBLIC, PRIVATE
# or INTERFACE) for every argument.
#
# If *FOLDER* option is set, it is organized into its name under the IDE if
# supported.
#
# [target_compile_definitions]: https://cmake.org/cmake/help/latest/command/target_compile_definitions.html
# [target_include_directories]: https://cmake.org/cmake/help/latest/command/target_include_directories.html
# [target_link_libraries]: https://cmake.org/cmake/help/latest/command/target_link_libraries.html
#

include(${CMAKE_CURRENT_LIST_DIR}/MolkoSetCompilerFlags.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/MolkoSetBuildDirectories.cmake)

function(molko_define_library)
	set(options EXTERNAL)
	set(oneValueArgs FOLDER TARGET TYPE)
	set(multiValueArgs ASSETS FLAGS INCLUDES LIBRARIES SOURCES TRANSLATIONS)

	cmake_parse_arguments(LIB "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if (NOT LIB_TARGET)
		message(FATAL_ERROR "Missing TARGET argument")
	endif ()
	if (NOT LIB_SOURCES)
		message(FATAL_ERROR "Missing SOURCES argument")
	endif ()

	molko_build_assets("${LIB_ASSETS}" ASSETS_OUTPUTS)

	if (LIB_TRANSLATIONS)
		molko_build_translations(
			TARGET ${LIB_TARGET}
			TRANSLATIONS ${LIB_TRANSLATIONS}
			SOURCES ${LIB_SOURCES}
			OUTPUTS NLS_OUTPUTS
		)
	endif ()

	add_library(
		${LIB_TARGET}
		${LIB_TYPE}
		${ASSETS_OUTPUTS}
		${LIB_SOURCES}
		${NLS_OUTPUTS}
	)

	if (NOT LIB_TYPE OR NOT ${LIB_TYPE} MATCHES INTERFACE)
		molko_set_build_directories(${LIB_TARGET})
		target_include_directories(${LIB_TARGET} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
	endif ()

	if (LIB_INCLUDES)
		target_include_directories(${LIB_TARGET} ${LIB_INCLUDES})
	endif ()
	if (LIB_FLAGS)
		target_compile_definitions(${LIB_TARGET} ${LIB_FLAGS})
	endif ()
	if (LIB_LIBRARIES)
		target_link_libraries(${LIB_TARGET} ${LIB_LIBRARIES})
	endif ()

	set_target_properties(
		${LIB_TARGET}
		PROPERTIES
			PREFIX ""
			IMPORT_PREFIX ""
			C_EXTENSIONS Off
			C_STANDARD 11
			C_STANDARD_REQUIRED On
	)

	if (LIB_FOLDER)
		set_target_properties(${LIB_TARGET} PROPERTIES FOLDER ${LIB_FOLDER})
	endif ()
endfunction()
