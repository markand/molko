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

macro(mlk_nls)
	set(options "")
	set(oneValueArgs "NAME")
	set(multiValueArgs "LANGS;SOURCES;OUTPUTS_VAR")

	cmake_parse_arguments(_nls "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if (NOT _nls_NAME)
		message(FATAL_ERROR "Missing NAME")
	elseif (NOT _nls_OUTPUTS_VAR)
		message(FATAL_ERROR "Missing OUTPUTS_VAR")
	elseif (NOT _nls_LANGS)
		message(FATAL_ERROR "Missing LANGS")
	elseif (NOT _nls_SOURCES)
		message(FATAL_ERROR "Missing SOURCES")
	endif ()

	set(_nls_po_cmds)

	foreach (l ${_nls_LANGS})
		cmake_path(GET l FILENAME name)
		cmake_path(REMOVE_EXTENSION name)

		add_custom_command(
			OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${name}.mo
			COMMAND NLS::msgfmt -o ${CMAKE_CURRENT_BINARY_DIR}/${name}.mo ${l}
			COMMENT "Generating NLS translation ${name}.mo"
			DEPENDS ${l}
		)

		list(APPEND ${_nls_OUTPUTS_VAR} ${CMAKE_CURRENT_BINARY_DIR}/${name}.mo)
		install(
			FILES ${CMAKE_CURRENT_BINARY_DIR}/${name}.mo
			DESTINATION ${CMAKE_INSTALL_LOCALEDIR}/${name}/LC_MESSAGES
			RENAME ${_nls_NAME}.mo
		)

		# Commands to update .po files.
		list(
			APPEND _nls_po_cmds
			COMMAND NLS::msgmerge --backup=off -qU ${l} ${CMAKE_CURRENT_SOURCE_DIR}/nls/${_nls_NAME}.pot
		)
	endforeach ()

	add_custom_target(
		po-${_nls_NAME}
		COMMAND
			NLS::xgettext
				--package-name "${LIB_NAME}"
				--package-version "${CMAKE_PROJECT_VERSION}"
				-cj
				-k_
				-kN_
				-LC
				-s
				-o ${CMAKE_CURRENT_SOURCE_DIR}/nls/${_nls_NAME}.pot ${_nls_SOURCES}
			${_nls_po_cmds}
		COMMENT "Updating in-source .pot and .po files"
	)
	set_target_properties(po-${_nls_NAME} PROPERTIES FOLDER "nls")
endmacro()
