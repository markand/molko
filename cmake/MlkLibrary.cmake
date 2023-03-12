#
# CMakeLists.txt -- CMake build system for Molko's Engine
#
# Copyright (c) 2020-2023 David Demelier <markand@malikania.fr>
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

include(${CMAKE_CURRENT_LIST_DIR}/MlkBcc.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/MlkNls.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/MlkMap.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/MlkTileset.cmake)

function(mlk_library)
	set(options "")
	set(oneValueArgs "NAME;FOLDER;TYPE")
	set(multiValueArgs "SOURCES;ASSETS;LANGS;LIBRARIES;INCLUDES;FLAGS;OPTIONS;MAPS;TILESETS")

	cmake_parse_arguments(LIB "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if (NOT LIB_NAME)
		message(FATAL_ERROR "Missing NAME")
	endif ()

	if (LIB_ASSETS)
		mlk_bcc(ASSETS ${LIB_ASSETS} OUTPUTS_VAR HEADERS)
	endif ()

	if (LIB_LANGS AND MLK_WITH_NLS)
		mlk_nls(
			NAME ${LIB_NAME}
			LANGS ${LIB_LANGS}
			SOURCES ${LIB_SOURCES}
			OUTPUTS_VAR MO
		)
	endif ()

	if (LIB_MAPS)
		mlk_maps("${LIB_MAPS}" ${CMAKE_CURRENT_BINARY_DIR}/maps maps)
	endif ()
	if (LIB_TILESETS)
		mlk_tilesets("${LIB_TILESETS}" ${CMAKE_CURRENT_BINARY_DIR}/tilesets tilesets)
	endif ()

	add_library(${LIB_NAME} ${LIB_TYPE} ${LIB_SOURCES} ${HEADERS} ${MO} ${maps} ${tilesets})

	if (LIB_FOLDER)
		set_target_properties(${LIB_NAME} PROPERTIES FOLDER extern)
	endif ()

	if (LIB_FLAGS)
		target_compile_definitions(${LIB_NAME} ${LIB_FLAGS})
	endif ()

	if (LIB_OPTIONS)
		target_compile_options(${LIB_NAME} ${LIB_OPTIONS})
	endif ()

	if (LIB_LIBRARIES)
		target_link_libraries(${LIB_NAME} ${LIB_LIBRARIES})
	endif ()

	if (LIB_INCLUDES)
		target_include_directories(${LIB_NAME} ${LIB_INCLUDES})
	endif ()

	set_target_properties(${LIB_NAME} PROPERTIES PREFIX "")
endfunction()
