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

project(libmlk-rexo)

add_library(libmlk-rexo INTERFACE ${libmlk-rexo_SOURCE_DIR}/rexo.h)
target_include_directories(
	libmlk-rexo
	INTERFACE
		$<BUILD_INTERFACE:${libmlk-rexo_SOURCE_DIR}>
)
set_target_properties(libmlk-rexo PROPERTIES FOLDER extern)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
	target_compile_options(libmlk-rexo INTERFACE -Wno-format)
endif ()

source_group("" FILES ${libmlk-rexo_SOURCE_DIR}/rexo.h)
