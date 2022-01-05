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

cmake_minimum_required(VERSION 3.20)
project(molko C)

set_property(GLOBAL PROPERTY USE_FOLDERS On)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED On)
set(CMAKE_C_EXTENSIONS Off)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
	set(CMAKE_C_FLAGS "-Wall -Wextra -Wno-deprecated-declarations -Wno-unknown-pragmas -pedantic -D_POSIX_C_SOURCE=200809L ${CMAKE_C_FLAGS}")

	if (CMAKE_C_COMPILER_ID MATCHES "GNU")
		set(CMAKE_C_FLAGS "-Wno-format-truncation ${CMAKE_C_FLAGS}")
	endif ()
elseif (CMAKE_C_COMPILER_ID MATCHES "MSVC")
	set(CMAKE_C_FLAGS "/W3 /wd4090 /wd4244 /wd4267 /wd4996 /wd5105 /wd6031 /wd6001 /wd26451 ${CMAKE_C_FLAGS}")
	set(CMAKE_C_FLAGS "/D_CRT_SECURE_NO_WARNINGS ${CMAKE_C_FLAGS}")
endif ()

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
	set(CMAKE_C_FLAGS "-D_DARWIN_C_SOURCE ${CMAKE_C_FLAGS}")
endif ()

option(MLK_WITH_EXAMPLES "Enable examples" Off)
option(MLK_WITH_JS "Enable Javascript bindings" On)
option(MLK_WITH_NLS "Enable NLS support" On)
option(MLK_WITH_TESTS "Enable unit tests" Off)
option(MLK_WITH_ZIP "Enable zip file support" On)
option(MLK_WITH_ZSTD "Enable zstd compression" On)

include(cmake/MlkBcc.cmake)
include(cmake/MlkExecutable.cmake)
include(cmake/MlkLibrary.cmake)
include(cmake/MlkMap.cmake)
include(cmake/MlkNls.cmake)
include(cmake/MlkTileset.cmake)

include(GNUInstallDirs)

find_package(SDL2 REQUIRED COMPONENTS image ttf)
find_package(OpenAL REQUIRED)
find_package(SndFile REQUIRED)
find_package(Jansson REQUIRED)

# POSIX math library isn't available everywhere.
find_library(M_LIBRARY m)

configure_file(
	${molko_SOURCE_DIR}/src/config.h.in
	${molko_BINARY_DIR}/src/config.h
)
include_directories(${molko_BINARY_DIR}/src)

if (MLK_WITH_JS)
	add_subdirectory(extern/libduktape)
endif ()

if (MLK_WITH_NLS)
	find_package(NLS REQUIRED)
	find_package(Intl REQUIRED)
endif ()

if (MLK_WITH_ZSTD)
	find_package(ZSTD REQUIRED)

	# Compressor is required as well.
	if (NOT TARGET ZSTD::exe)
		message(FATAL_ERROR "Missing zstd command line utility")
	endif ()
endif ()

if (MLK_WITH_ZIP)
	find_package(Libzip REQUIRED)
endif ()

add_subdirectory(extern/libsqlite)
add_subdirectory(extern/librexo)

add_subdirectory(src/tools/bcc)
add_subdirectory(src/tools/tileset)
add_subdirectory(src/tools/map)

add_subdirectory(src/libmlk-port)
add_subdirectory(src/libmlk-core)
add_subdirectory(src/libmlk-ui)
add_subdirectory(src/libmlk-rpg)

if (MLK_WITH_JS)
	add_subdirectory(src/libmlk-core-js)
	add_subdirectory(src/mlk-run)
endif ()

if (MLK_WITH_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif ()

if (MLK_WITH_EXAMPLES)
	add_subdirectory(examples)
endif ()
