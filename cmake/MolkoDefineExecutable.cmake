#
# MolkoDefineTest.cmake -- CMake build system for molko
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
# # molko_define_executable
#
# Create an executable.
#
# ## Synopsis
#
# ```cmake
# molko_define_test(
#   TARGET              target name
#   SOURCES             src1, src2, srcn
#   FOLDER              (Optional) IDE folder if supported
#   TRANSLATIONS        (Optional) list of translations
#   ASSETS              (Optional) list of assets to build
#   FLAGS               (Optional) C flags (without -D)
#   LIBRARIES           (Optional) libraries to link
#   INCLUDES            (Optional) includes
#   INSTALL             (Optional) create install target
# )
# ```
#
# Create an executable with the name *TARGET* with the given *SOURCES*.
#
# Optional include paths, libraries and flags can be specified via *INCLUDES*,
# *LIBRARIES* and *FLAGS* arguments respectively.
#
# If argument *ASSETS* is set, they are generated in the target binary
# directory.
#
# The optional argument *TRANSLATIONS* should contain a list of supported
# translations. See molko_build_translations for more info.
#

include(${CMAKE_CURRENT_LIST_DIR}/MolkoBuildAssets.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/MolkoBuildTranslations.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/MolkoSetCompilerFlags.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/MolkoSetBuildDirectories.cmake)

function(molko_define_executable)
	set(options INSTALL)
	set(oneValueArgs FOLDER TARGET)
	set(multiValueArgs ASSETS FLAGS INCLUDES LIBRARIES SOURCES TRANSLATIONS)

	cmake_parse_arguments(EXE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if (NOT EXE_TARGET)
		message(FATAL_ERROR "Missing TARGET argument")
	endif ()
	if (NOT EXE_SOURCES)
		message(FATAL_ERROR "Missing SOURCES argument")
	endif ()

	molko_build_assets("${EXE_ASSETS}" OUTPUTS)

	if (EXE_TRANSLATIONS)
		molko_build_translations(
			TARGET ${EXE_TARGET}
			TRANSLATIONS ${EXE_TRANSLATIONS}
			SOURCES ${EXE_SOURCES}
		)
	endif ()

	add_executable(${EXE_TARGET} ${EXE_SOURCES} ${OUTPUTS})
	target_compile_definitions(${EXE_TARGET} PRIVATE ${EXE_FLAGS})
	target_include_directories(
		${EXE_TARGET}
		PRIVATE
			${CMAKE_CURRENT_BINARY_DIR}
			${EXE_INCLUDES}
	)
	target_link_libraries(
		${EXE_TARGET}
		PRIVATE
			${EXE_LIBRARIES}
	)

	if (EXE_FOLDER)
		set_target_properties(${EXE_TARGET} PROPERTIES FOLDER ${EXE_FOLDER})
	endif ()
	if (EXE_INSTALL)
		install(TARGETS ${EXE_TARGET} DESTINATION ${CMAKE_INSTALL_BINDIR})
	endif ()

	molko_set_compiler_flags(${EXE_TARGET})
	molko_set_build_directories(${EXE_TARGET})
endfunction()
