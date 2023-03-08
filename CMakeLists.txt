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

cmake_minimum_required(VERSION 3.20)
project(
	molko
	VERSION "0.1.0"
	DESCRIPTION "Molko's Engine"
	HOMEPAGE_URL "http://hg.malikania.fr/molko"
	LANGUAGES "C"
)

set_property(GLOBAL PROPERTY USE_FOLDERS On)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED On)
set(CMAKE_C_EXTENSIONS On)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
	set(CMAKE_C_FLAGS "-Wall -Wextra -Wno-deprecated-declarations -Wno-unknown-pragmas ${CMAKE_C_FLAGS}")

	if (CMAKE_C_COMPILER_ID MATCHES "GNU")
		set(CMAKE_C_FLAGS "-Wno-format-truncation ${CMAKE_C_FLAGS}")
	else ()
		set(CMAKE_C_FLAGS "-Wno-fixed-enum-extension ${CMAKE_C_FLAGS}")
	endif ()
elseif (CMAKE_C_COMPILER_ID MATCHES "MSVC")
	set(CMAKE_C_FLAGS "/W3 /wd4090 /wd4244 /wd4267 /wd4996 /wd5105 /wd6031 /wd6001 /wd26451 ${CMAKE_C_FLAGS}")
	set(CMAKE_C_FLAGS "/D_CRT_SECURE_NO_WARNINGS ${CMAKE_C_FLAGS}")
endif ()

option(MLK_WITH_DOXYGEN "Enable doxygen documentation" On)
option(MLK_WITH_EXAMPLES "Enable examples" On)
option(MLK_WITH_NLS "Enable NLS support" Off)
option(MLK_WITH_TESTS "Enable unit tests" On)

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

if (MLK_WITH_NLS)
	find_package(Intl)
endif ()

add_subdirectory(extern/libsqlite)
add_subdirectory(extern/libdt)

if (MLK_WITH_DOXYGEN)
	add_subdirectory(doc)
endif ()

add_subdirectory(mlk-bcc)
add_subdirectory(mlk-tileset)
add_subdirectory(mlk-map)

add_subdirectory(libmlk-util)
add_subdirectory(libmlk-core)
add_subdirectory(libmlk-ui)
add_subdirectory(libmlk-rpg)

if (MLK_WITH_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif ()

if (MLK_WITH_EXAMPLES)
	add_subdirectory(libmlk-example)
	add_subdirectory(examples)
endif ()
