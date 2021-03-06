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

function(mlk_executable)
	set(options "INSTALL")
	set(oneValueArgs "NAME;FOLDER")
	set(multiValueArgs "ASSETS;SOURCES;LIBRARIES;INCLUDES;FLAGS")

	cmake_parse_arguments(EXE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if (NOT EXE_NAME)
		message(FATAL_ERROR "Missing NAME")
	endif ()

	if (EXE_ASSETS)
		mlk_bcc(ASSETS ${EXE_ASSETS} OUTPUTS_VAR HEADERS)
		source_group(build/assets FILES ${HEADERS})
	endif ()

	add_executable(${EXE_NAME} ${EXE_SOURCES} ${HEADERS})
	target_include_directories(${EXE_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

	if (EXE_FOLDER)
		set_target_properties(${EXE_NAME} PROPERTIES FOLDER ${EXE_FOLDER})
	endif ()

	if (EXE_LIBRARIES)
		target_link_libraries(${EXE_NAME} ${EXE_LIBRARIES})
	endif ()

	if (EXE_INCLUDES)
		target_include_directories(${EXE_NAME} ${EXE_INCLUDES})
	endif ()

	if (EXE_INSTALL)
		install(TARGETS ${EXE_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
	endif ()
endfunction()
