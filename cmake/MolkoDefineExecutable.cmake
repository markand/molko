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

include(${CMAKE_CURRENT_LIST_DIR}/MolkoBuildAssets.cmake)

function(molko_define_executable)
	set(options)
	set(oneValueArgs FOLDER TARGET)
	set(multiValueArgs ASSETS FLAGS INCLUDES LIBRARIES SOURCES)

	cmake_parse_arguments(EXE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if (NOT EXE_TARGET)
		message(FATAL_ERROR "Missing TARGET argument")
	endif ()
	if (NOT EXE_SOURCES)
		message(FATAL_ERROR "Missing SOURCES argument")
	endif ()

	molko_build_assets("${EXE_ASSETS}" OUTPUTS)

	add_executable(${EXE_TARGET} ${EXE_SOURCES} ${OUTPUTS})
	target_compile_definitions(${EXE_TARGET} PRIVATE ${EXE_FLAGS})
	target_include_directories(${EXE_TARGET} PRIVATE ${EXE_INCLUDES})
	target_link_libraries(
		${EXE_TARGET}
		PRIVATE
			${EXE_LIBRARIES}
	)

	if (EXE_FOLDER)
		set_target_properties(${EXE_TARGET} PROPERTIES FOLDER ${EXE_FOLDER})
	endif ()
endfunction()
