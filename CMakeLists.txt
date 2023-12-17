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

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED On)
set(CMAKE_C_EXTENSIONS On)
set(CMAKE_MODULE_PATH "${molko_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
	set(CMAKE_C_FLAGS "-Wall -Wextra -Wno-deprecated-declarations -Wno-unknown-pragmas ${CMAKE_C_FLAGS}")

	if (CMAKE_C_COMPILER_ID MATCHES "GNU")
		set(CMAKE_C_FLAGS "-Wno-format-truncation ${CMAKE_C_FLAGS}")
	else ()
		set(CMAKE_C_FLAGS "-Wno-fixed-enum-extension ${CMAKE_C_FLAGS}")
	endif ()

	#
	# For some reason, Xcode generates dozens of more warnings that are
	# sometimes a bit too naive
	#
	if (CMAKE_GENERATOR MATCHES "Xcode")
		set(CMAKE_C_FLAGS "-Wno-shorten-64-to-32 ${CMAKE_C_FLAGS}")
	endif ()
elseif (CMAKE_C_COMPILER_ID MATCHES "MSVC")
	set(CMAKE_C_FLAGS "/D_CRT_SECURE_NO_WARNINGS ${CMAKE_C_FLAGS}")
endif ()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

#
# Create an option MLK_WITH_<name> as cache entry.
#
function(mlk_option name value type help)
	set(MLK_WITH_${name} ${value} CACHE ${type} ${help})
endfunction()

include(cmake/MlkOptions.cmake)
include(cmake/MlkBcc.cmake)
include(cmake/MlkExecutable.cmake)
include(cmake/MlkLibrary.cmake)
include(cmake/MlkMap.cmake)
include(cmake/MlkNls.cmake)
include(cmake/MlkTileset.cmake)

find_package(Jansson REQUIRED)
find_package(OpenAL REQUIRED)
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SndFile REQUIRED)

# POSIX math library isn't available everywhere.
find_library(M_LIBRARY m)

if (MLK_WITH_NLS)
	find_package(NLS REQUIRED)
	find_package(Intl REQUIRED)
endif ()

if (MLK_WITH_ZIP)
	find_package(ZIP REQUIRED)
endif ()

add_subdirectory(extern/libsqlite)
add_subdirectory(extern/libdt)
add_subdirectory(extern/libutlist)

if (MLK_WITH_JAVASCRIPT)
	add_subdirectory(extern/libduktape)
endif ()

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

if (MLK_WITH_JAVASCRIPT)
	add_subdirectory(mlk-run)
endif ()

configure_file(
	${molko_SOURCE_DIR}/cmake/MlkOptions.install.cmake
	${molko_BINARY_DIR}/cmake/MlkOptions.cmake
)
install(
	FILES
		${molko_BINARY_DIR}/cmake/MlkOptions.cmake
		${molko_SOURCE_DIR}/cmake/FindSndFile.cmake
		${molko_SOURCE_DIR}/cmake/FindZIP.cmake
		${molko_SOURCE_DIR}/cmake/MlkBcc.cmake
		${molko_SOURCE_DIR}/cmake/MlkMap.cmake
		${molko_SOURCE_DIR}/cmake/MlkTileset.cmake
	DESTINATION "${MLK_WITH_CMAKEDIR}/mlk"
)
