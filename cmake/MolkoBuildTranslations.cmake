#
# MolkoBuildTranslations.cmake -- CMake build system for molko
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
# # molko_build_translations
#
# Build translations and update them.
#
# ## Synopsis
#
# ```cmake
# molko_build_translations(
#   TARGET              target name
#   SOURCES             target sources
#   OUTPUTS             output variable
#   TRANSLATIONS        list of localizations
# )
# ```
#
# Generate target and output commands for NLS (via GNU gettext) support for the
# given *TARGET* name.
#
# The argument *SOURCES* must contain sources to extract gettext keywords, it
# will search for _, N_. The list of *SOURCES* can contain any files, only .c
# and .h will be filtered.
#
# The argument *OUTPUTS* will be set with the generated .mo files in the binary
# directory and installed to *CMAKE_INSTALL_LOCALEDIR*.
#
# The argument *TRANSLATIONS* should contain a list of languages supported in the
# gettext form (ll_LL@variant, see ISO 639 and ISO 3166 for more details).
#
# This macro create a `<TARGET>-po` target that will recreate the .pot file and
# every .po files in the nls/ directory for each language specified in
# *TRANSLATIONS*. Note, if you add a new language into translations but do not
# copy the .pot file, a warning will be issued and you should copy the .pot
# file as the new .po language file.
#
# Since the target is modifying files directly in the source tree they are not
# included in any build process and must be invoked manually.
#

if (MOLKO_WITH_NLS)
	find_program(XGETTEXT_EXE xgettext)
	find_program(MSGMERGE_EXE msgmerge)
	find_program(MSGFMT_EXE msgfmt)
endif ()

if (MOLKO_WITH_NLS AND XGETTEXT_EXE AND MSGMERGE_EXE)
	macro(molko_build_translations)
		set(options)
		set(oneValueArgs TARGET OUTPUTS)
		set(multiValueArgs SOURCES TRANSLATIONS)

		cmake_parse_arguments(NLS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

		if (NOT NLS_OUTPUTS)
			message(FATAL_ERROR "Missing OUTPUTS argument")
		endif ()
		if (NOT NLS_TARGET)
			message(FATAL_ERROR "Missing TARGET argument")
		endif ()
		if (NOT NLS_SOURCES)
			message(FATAL_ERROR "Missing SOURCES argument")
		endif ()

		# Remove non C files.
		list(FILTER NLS_SOURCES INCLUDE REGEX "\\.[ch$]")
		set(pot ${CMAKE_CURRENT_SOURCE_DIR}/nls/${NLS_TARGET}.pot)

		# First command to generate the .pot.
		list(
			APPEND
			commands
				COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/nls
				COMMAND ${XGETTEXT_EXE} -cj -k_ -kN_ -LC -s -o ${pot} ${NLS_SOURCES}
		)

		#
		# For every translation create a msgmerge command and output
		# file.
		#
		set(outputs)

		foreach (t ${NLS_TRANSLATIONS})
			set(po ${CMAKE_CURRENT_SOURCE_DIR}/nls/${t}.po)

			#
			# If library dir isn't absolute, install them in the
			# build directory so we can use translations during
			# development.
			#
			if (NOT IS_ABSOLUTE ${CMAKE_INSTALL_LIBDIR})
				set(modir ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${CMAKE_INSTALL_LOCALEDIR}/${t}/LC_MESSAGES)
				set(mo ${modir}/${NLS_TARGET}.mo)
			else ()
				set(modir ${CMAKE_CURRENT_BINARY_DIR}/${t})
				set(mo ${CMAKE_CURRENT_BINARY_DIR}/${NLS_TARGET}.mo)
			endif ()

			if (NOT EXISTS ${po})
				message(WARNING "Missing translation ${po}")
			endif ()

			# Commands to generate .po files from the .pot.
			list(APPEND commands COMMAND ${MSGMERGE_EXE} --backup=off -q -U ${po} ${pot})

			# Finally generate a .mo output from po file.
			add_custom_command(
				OUTPUT ${mo}
				VERBATIM
				COMMAND ${CMAKE_COMMAND} -E make_directory ${modir}
				COMMAND ${MSGFMT_EXE} -o ${mo} ${po}
				DEPENDS ${po}
				COMMENT "Generating translation binary ${mo}"
			)

			list(APPEND outputs ${mo})

			install(
				FILES ${mo}
				DESTINATION ${CMAKE_INSTALL_LOCALEDIR}/${t}/LC_MESSAGES
				RENAME ${NLS_TARGET}.mo
			)
		endforeach ()

		set(${NLS_OUTPUTS} ${outputs})
		source_group("mo" FILES ${outputs})

		# User target to update .pot and .po files.
		add_custom_target(
			${NLS_TARGET}-po
			VERBATIM
			COMMENT "Generating translations for ${NLS_TARGET}"
			${commands}
		)

		set_target_properties(${NLS_TARGET}-po PROPERTIES FOLDER nls)
	endmacro()
else ()
	function(molko_build_translations)
	endfunction()
endif ()
