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

project(tests)

set(
	TESTS
	action
	action-script
	alloc
	color
	drawable
	save
	save-quest
	state
	tileset
	util
)

foreach (t ${TESTS})
	add_executable(test-${t} ${tests_SOURCE_DIR}/test-${t}.c)
	add_test(NAME test-${t} COMMAND test-${t})
	target_link_libraries(test-${t} libmlk-rpg libmlk-dt)
	target_compile_definitions(test-${t} PRIVATE DIRECTORY="${tests_SOURCE_DIR}/assets")
	set_target_properties(test-${t} PROPERTIES FOLDER tests)
	source_group("" FILES test-${t}.c)

	if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
		target_compile_options(test-${t} PRIVATE -Wno-unused-parameter)
	endif ()
endforeach ()
