#
# CMakeLists.txt -- CMake build system for libui
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

project(libui)

set(
	ASSETS
	${libui_SOURCE_DIR}/ui/assets/fonts/opensans-light.ttf
	${libui_SOURCE_DIR}/ui/assets/fonts/opensans-regular.ttf
)

set(
	SOURCES
	${libui_SOURCE_DIR}/ui/align.c
	${libui_SOURCE_DIR}/ui/align.h
	${libui_SOURCE_DIR}/ui/button.c
	${libui_SOURCE_DIR}/ui/button.h
	${libui_SOURCE_DIR}/ui/checkbox.c
	${libui_SOURCE_DIR}/ui/checkbox.h
	${libui_SOURCE_DIR}/ui/debug.c
	${libui_SOURCE_DIR}/ui/debug.h
	${libui_SOURCE_DIR}/ui/frame.c
	${libui_SOURCE_DIR}/ui/frame.h
	${libui_SOURCE_DIR}/ui/label.c
	${libui_SOURCE_DIR}/ui/label.h
	${libui_SOURCE_DIR}/ui/theme.c
	${libui_SOURCE_DIR}/ui/theme.h
	${libui_SOURCE_DIR}/ui/ui.c
	${libui_SOURCE_DIR}/ui/ui.h
)

molko_define_library(
	TARGET libui
	SOURCES ${SOURCES}
	ASSETS ${ASSETS}
	LIBRARIES
		libcore
	PUBLIC_INCLUDES
		$<BUILD_INTERFACE:${libui_SOURCE_DIR}>
)

source_group(ui FILES ${SOURCES})
