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

project(libmlk-data)

#
# The following projects use the data:
#
# - examples,
# - libmlk-adventure (implies mlk-adventure),
# - tests.
#

set(
	DIRECTORIES
	fonts
	images
	maps
	music
	sounds
	sprites
)

# Directories to not link but create instead (for converting files).
set(NO_LINK maps)

set(
	MAPS
	${libmlk-data_SOURCE_DIR}/maps/map-world.json
)

set(
	TILESETS
	${libmlk-data_SOURCE_DIR}/maps/tileset-world.json
)

if (NOT IS_ABSOLUTE ${CMAKE_INSTALL_DATADIR})
	set(directories)

	foreach (d ${DIRECTORIES})
		set(outputdir ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${CMAKE_INSTALL_DATADIR}/mlk-adventure)

		# If no link, copy the whole directory instead.
		if (${d} IN_LIST NO_LINK)
			add_custom_command(
				OUTPUT ${outputdir}/${d}
				COMMENT "Copying data ${d} to binary directory ${outputdir}"
				VERBATIM
				DEPENDS ${libmlk-data_SOURCE_DIR}/${d}
				COMMAND
					${CMAKE_COMMAND} -E copy_directory ${libmlk-data_SOURCE_DIR}/${d} ${outputdir}/${d}
			)
		else ()

			add_custom_command(
				OUTPUT ${outputdir}/${d}
				COMMENT "Linking data ${d} to binary directory ${outputdir}"
				VERBATIM
				DEPENDS ${libmlk-data_SOURCE_DIR}/${d}
				COMMAND
					${CMAKE_COMMAND} -E make_directory ${outputdir}
				COMMAND
					${CMAKE_COMMAND} -E create_symlink ${libmlk-data_SOURCE_DIR}/${d} ${outputdir}/${d}
			)
		endif ()

		list(APPEND directories ${outputdir}/${d})
	endforeach ()
endif ()

foreach (d ${DIRECTORIES})
	install(
		DIRECTORY ${libmlk-data_SOURCE_DIR}/${d}
		DESTINATION ${CMAKE_INSTALL_DATADIR}/mlk-adventure
		PATTERN REGEX "\\.(json|xcf)$" EXCLUDE
	)
endforeach ()

# Convert maps and tilesets too.
molko_build_maps(
	OUTPUT_DIR mlk-adventure/maps
	OUTPUTS objects
	MAPS ${MAPS}
	TILESETS ${TILESETS}
)

add_library(libmlk-data INTERFACE ${directories} ${objects})
