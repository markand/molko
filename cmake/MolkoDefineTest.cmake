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
# # molko_define_test
#
# Create unit test.
#
# ## Synopsis
#
# ```cmake
# molko_define_test(
#   TARGET      target name
#   SOURCES     src1, src2, srcn
#   ASSETS      (Optional) list of assets to build
#   FLAGS       (Optional) C flags (without -D)
#   LIBRARIES   (Optional) libraries to link
#   INCLUDES    (Optional) includes
# )
# ```
#
# Create an executable with the name *TARGET* and a test case of the same name
# with the given *SOURCES*.
#
# Optional include paths, libraries and flags can be specified via *INCLUDES*,
# *LIBRARIES* and *FLAGS* arguments respectively.
#
# If argument *ASSETS* is set, they are generated in the target binary
# directory.
#

include(${CMAKE_CURRENT_LIST_DIR}/MolkoBuildAssets.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/MolkoSetCompilerFlags.cmake)

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
	target_include_directories(
		test-${TEST_TARGET}
		PRIVATE
			${CMAKE_CURRENT_BINARY_DIR}
			${TEST_INCLUDES}
	)
	target_link_libraries(
		test-${TEST_TARGET}
		PRIVATE
			libmlk-core
			libmlk-adventure
			libgreatest
			${TEST_LIBRARIES}
	)

	add_test(NAME ${TEST_TARGET} COMMAND test-${TEST_TARGET})
	set_target_properties(test-${TEST_TARGET} PROPERTIES FOLDER tests)

	molko_set_compiler_flags(test-${TEST_TARGET})
endfunction()
