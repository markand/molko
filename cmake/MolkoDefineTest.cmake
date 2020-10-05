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

function(molko_define_test)
	set(options)
	set(oneValueArgs TARGET)
	set(multiValueArgs ASSETS FLAGS INCLUDES LIBRARIES SOURCES)

	cmake_parse_arguments(TEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if (NOT TEST_TARGET)
		message(FATAL_ERROR "Missing TARGET argument")
	endif ()
	if (NOT TEST_SOURCES)
		message(FATAL_ERROR "Missing SOURCES argument")
	endif ()

	molko_build_assets("${TEST_ASSETS}" OUTPUTS)

	add_executable(test-${TEST_TARGET} ${TEST_SOURCES} ${OUTPUTS})
	target_compile_definitions(test-${TEST_TARGET} PRIVATE ${TEST_FLAGS})
	target_include_directories(test-${TEST_TARGET} PRIVATE ${TEST_INCLUDES})
	target_link_libraries(
		test-${TEST_TARGET}
		PRIVATE
			libcore
			libadventure
			libgreatest
			${TEST_LIBRARIES}
	)
	add_test(NAME ${TEST_TARGET} COMMAND test-${TEST_TARGET})
endfunction()
