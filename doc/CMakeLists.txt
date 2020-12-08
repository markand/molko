#
# CMakeLists.txt -- CMake build system for molko
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

project(doc)

find_program(MKDOCS_EXE mkdocs DOC "Path to mkdocs")
find_program(SH_EXE sh DOC "Path to a POSIX shell")

if (MKDOCS_EXE)
	set(
		DOC_SOURCES
		${doc_SOURCE_DIR}/docs/about.md
		${doc_SOURCE_DIR}/docs/cmake/MolkoBuildAssets.md
		${doc_SOURCE_DIR}/docs/cmake/MolkoBuildMaps.md
		${doc_SOURCE_DIR}/docs/cmake/MolkoBuildTranslations.md
		${doc_SOURCE_DIR}/docs/cmake/MolkoDefineExecutable.md
		${doc_SOURCE_DIR}/docs/cmake/MolkoDefineLibrary.md
		${doc_SOURCE_DIR}/docs/cmake/MolkoDefineTest.md
		${doc_SOURCE_DIR}/docs/cmake/MolkoSetBuildDirectories.md
		${doc_SOURCE_DIR}/docs/cmake/MolkoSetCompilerFlags.md
		${doc_SOURCE_DIR}/docs/dev/api/core/action.md
		${doc_SOURCE_DIR}/docs/dev/api/core/alloc.md
		${doc_SOURCE_DIR}/docs/dev/api/core/animation.md
		${doc_SOURCE_DIR}/docs/dev/api/core/clock.md
		${doc_SOURCE_DIR}/docs/dev/api/core/color.md
		${doc_SOURCE_DIR}/docs/dev/api/core/core.md
		${doc_SOURCE_DIR}/docs/dev/api/core/drawable.md
		${doc_SOURCE_DIR}/docs/dev/api/core/event.md
		${doc_SOURCE_DIR}/docs/dev/api/core/error.md
		${doc_SOURCE_DIR}/docs/dev/api/core/font.md
		${doc_SOURCE_DIR}/docs/dev/api/core/game.md
		${doc_SOURCE_DIR}/docs/dev/api/core/image.md
		${doc_SOURCE_DIR}/docs/dev/api/core/inhibit.md
		${doc_SOURCE_DIR}/docs/dev/api/core/key.md
		${doc_SOURCE_DIR}/docs/dev/api/core/maths.md
		${doc_SOURCE_DIR}/docs/dev/api/core/mouse.md
		${doc_SOURCE_DIR}/docs/dev/api/core/music.md
		${doc_SOURCE_DIR}/docs/dev/api/core/painter.md
		${doc_SOURCE_DIR}/docs/dev/api/core/panic.md
		${doc_SOURCE_DIR}/docs/dev/api/core/save.md
		${doc_SOURCE_DIR}/docs/dev/api/core/script.md
		${doc_SOURCE_DIR}/docs/dev/api/core/sound.md
		${doc_SOURCE_DIR}/docs/dev/api/core/sprite.md
		${doc_SOURCE_DIR}/docs/dev/api/core/state.md
		${doc_SOURCE_DIR}/docs/dev/api/core/sys.md
		${doc_SOURCE_DIR}/docs/dev/api/core/texture.md
		${doc_SOURCE_DIR}/docs/dev/api/core/trace.md
		${doc_SOURCE_DIR}/docs/dev/api/core/translate.md
		${doc_SOURCE_DIR}/docs/dev/api/core/util.md
		${doc_SOURCE_DIR}/docs/dev/api/core/window.md
		${doc_SOURCE_DIR}/docs/dev/error.md
		${doc_SOURCE_DIR}/docs/dev/faq.md
		${doc_SOURCE_DIR}/docs/dev/howto/01-init.md
		${doc_SOURCE_DIR}/docs/dev/ownership.md
		${doc_SOURCE_DIR}/docs/index.md
		${doc_SOURCE_DIR}/docs/install.md
		${doc_SOURCE_DIR}/docs/specs/map.md
		${doc_SOURCE_DIR}/docs/specs/tileset.md
		${doc_SOURCE_DIR}/docs/tools/bcc.md
		${doc_SOURCE_DIR}/docs/tools/map.md
		${doc_SOURCE_DIR}/docs/tools/tileset.md
		${doc_SOURCE_DIR}/mkdocs.yml
	)

	add_custom_target(
		doc
		VERBATIM
		SOURCES ${DOC_SOURCES}
		WORKING_DIRECTORY ${doc_SOURCE_DIR}
		COMMAND
			${MKDOCS_EXE} build -d ${doc_BINARY_DIR}/html
	)

	add_custom_target(
		doc-serve
		VERBATIM
		SOURCES ${DOC_SOURCES}
		WORKING_DIRECTORY ${doc_SOURCE_DIR}
		COMMAND
			${MKDOCS_EXE} serve
	)

	set_target_properties(doc doc-serve PROPERTIES FOLDER doc)
	source_group(TREE ${doc_SOURCE_DIR} FILES ${DOC_SOURCES})
endif ()

if (SH_EXE)
	# Our CMake macros are extracted from the source code itself.
	set(
		DOC_MACROS
		${cmake_SOURCE_DIR}/MolkoBuildAssets.cmake
		${cmake_SOURCE_DIR}/MolkoBuildMaps.cmake
		${cmake_SOURCE_DIR}/MolkoBuildTranslations.cmake
		${cmake_SOURCE_DIR}/MolkoDefineExecutable.cmake
		${cmake_SOURCE_DIR}/MolkoDefineLibrary.cmake
		${cmake_SOURCE_DIR}/MolkoDefineTest.cmake
		${cmake_SOURCE_DIR}/MolkoSetBuildDirectories.cmake
		${cmake_SOURCE_DIR}/MolkoSetCompilerFlags.cmake
	)

	foreach (m ${DOC_MACROS})
		get_filename_component(file ${m} NAME_WE)
		set(output ${doc_SOURCE_DIR}/docs/cmake/${file}.md)

		list(
			APPEND commands
			COMMAND
				${SH_EXE} ${doc_SOURCE_DIR}/scripts/cmake-extract.sh ${m} > ${output}
		)
	endforeach ()

	# Create a non-ALL target to update CMake documentation in the source
	# tree.
	add_custom_target(
		doc-cmake
		VERBATIM
		DEPENDS ${doc_SOURCE_DIR}/scripts/cmake-extract.sh
		COMMENT "Updating in-source ${doc_SOURCE_DIR}/docs/cmake"
		COMMAND ${commands}
	)

	set_target_properties(doc-cmake PROPERTIES FOLDER doc)
endif ()
