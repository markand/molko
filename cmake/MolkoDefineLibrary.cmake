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
# molko_define_library
# --------------------
#
# Synopsis:
#
# molko_define_library(
#   TARGET              target name
#   SOURCES             src1, src2, srcn
#   EXTERNAL            (Optional) set to true for external libraries
#   FOLDER              (Optional) optional subfolder to organize
#   TYPE                (Optional) type of library
#   ASSETS              (Optional) list of assets
#   LIBRARIES           (Optional) libraries to link
#   PRIVATE_FLAGS       (Optional) C flags (without -D)
#   PRIVATE_INCLUDES    (Optional) local includes for the target only
#   PUBLIC_FLAGS        (Optional) C flags (without -D)
#   PUBLIC_INCLUDES     (Optional) includes to share with target dependencies
# )
#
# Create a library and optionally install it.
#
# The function create a new library named with the parameter TARGET, you should
# prefix it with "lib" as its the convention within molko (e.g. libfoo), the
# prefix is automatically removed.
#
# The argument SOURCES should contains the C source files and HEADERS should
# points to a directory to be installed verbatim in the include directory.
#
# Optional argument EXTERNAL should be set for targets that are not maintained
# here (e.g. third party libraries embedded).
#
# Optional argument PRIVATE_FLAGS, PUBLIC_FLAGS, PRIVATE_INCLUDES,
# PUBLIC_INCLUDES, LIBRARIES may be passed to set compile flags, includes and
# libraries respectively.
#
# The arguments ASSETS contains a list of assets to be converted during the
# build. The file hierarchy is conserved in the ${CMAKE_CURRENT_BINARY_DIR}.
#
# If FOLDER option is set, it is organized into its name under the IDE if
# supported.
#

include(${CMAKE_CURRENT_LIST_DIR}/MolkoBuildAssets.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/MolkoBuildTilesets.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/MolkoBuildMaps.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/MolkoSetCompilerFlags.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/MolkoSetBuildDirectories.cmake)

function(molko_define_library)
	set(options EXTERNAL)
	set(oneValueArgs FOLDER TARGET TYPE)
	set(multiValueArgs ASSETS LIBRARIES MAPS PRIVATE_FLAGS PRIVATE_INCLUDES PUBLIC_FLAGS PUBLIC_INCLUDES TILESETS SOURCES TRANSLATIONS)

	cmake_parse_arguments(LIB "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if (NOT LIB_TARGET)
		message(FATAL_ERROR "Missing TARGET argument")
	endif ()
	if (NOT LIB_SOURCES)
		message(FATAL_ERROR "Missing SOURCES argument")
	endif ()

	molko_build_assets("${LIB_ASSETS}" ASSETS_OUTPUTS)
	molko_build_tilesets("${LIB_TILESETS}" TILESETS_OUTPUTS)
	molko_build_maps("${LIB_MAPS}" MAPS_OUTPUTS)

	if (${LIB_TYPE} MATCHES "INTERFACE")
		add_library(${LIB_TARGET} INTERFACE)
		target_sources(
			${LIB_TARGET}
			INTERFACE
				${LIB_SOURCES}
				${ASSETS_OUTPUTS}
				${MAPS_OUTPUTS}
				${TILESETS_OUTPUTS}
		)
		target_include_directories(
			${LIB_TARGET}
			INTERFACE
				${LIB_PRIVATE_INCLUDES}
				${LIB_PUBLIC_INCLUDES}
		)
	else ()
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
			${LIB_SOURCES}
			${ASSETS_OUTPUTS}
			${MAPS_OUTPUTS}
			${TILESETS_OUTPUTS}
			${NLS_OUTPUTS}
		)

		target_include_directories(
			${LIB_TARGET}
			PRIVATE
				${LIB_PRIVATE_INCLUDES}
			PUBLIC
				$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
				${LIB_PUBLIC_INCLUDES}
		)
		target_compile_definitions(
			${LIB_TARGET}
			PRIVATE
				${LIB_PRIVATE_FLAGS}
			PUBLIC
				${LIB_PUBLIC_FLAGS}
		)
		target_link_libraries(${LIB_TARGET} ${LIB_LIBRARIES})
		set_target_properties(
			${LIB_TARGET}
			PROPERTIES
				PREFIX ""
				IMPORT_PREFIX ""
				C_EXTENSIONS Off
				C_STANDARD 11
				C_STANDARD_REQUIRED On
		)

		if (NOT ${LIB_EXTERNAL})
			molko_set_compiler_flags(${LIB_TARGET})
		endif ()
	endif ()

	molko_set_build_directories(${LIB_TARGET})

	if (LIB_FOLDER)
		set_target_properties(${LIB_TARGET} PROPERTIES FOLDER ${LIB_FOLDER})
	endif ()
endfunction()
