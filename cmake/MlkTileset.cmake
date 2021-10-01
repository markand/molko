#
# CMakeLists.txt -- CMake build system for Molko's Adventure
#
# Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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

function(mlk_tileset input output)
	if (MLK_WITH_ZSTD)
		set(cmd
			COMMAND $<TARGET_FILE:mlk-tileset> < ${input} > ${output}.zst
			COMMAND ZSTD::exe -17 -fq --rm ${output}.zst -o ${output}
		)
	else ()
		set(cmd COMMAND $<TARGET_FILE:mlk-tileset> < ${input} > ${output})
	endif ()

	add_custom_command(
		OUTPUT ${output}
		COMMAND ${cmd}
		DEPENDS $<TARGET_FILE:mlk-tileset>
		COMMENT "Generating ${output}"
	)
endfunction()
