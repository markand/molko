#
# CMakeLists.txt -- CMake build system for libmlk-ui
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

project(libmlk-ui)

set(
	ASSETS
	${libmlk-ui_SOURCE_DIR}/assets/fonts/opensans-light.ttf
	${libmlk-ui_SOURCE_DIR}/assets/fonts/opensans-regular.ttf
)

set(
	PO
	${libmlk-ui_SOURCE_DIR}/nls/fr.po
	${libmlk-ui_SOURCE_DIR}/nls/libmlk-ui.pot
)

set(
	SOURCES
	${libmlk-ui_SOURCE_DIR}/ui/align.c
	${libmlk-ui_SOURCE_DIR}/ui/align.h
	${libmlk-ui_SOURCE_DIR}/ui/button.c
	${libmlk-ui_SOURCE_DIR}/ui/button.h
	${libmlk-ui_SOURCE_DIR}/ui/checkbox.c
	${libmlk-ui_SOURCE_DIR}/ui/checkbox.h
	${libmlk-ui_SOURCE_DIR}/ui/debug.c
	${libmlk-ui_SOURCE_DIR}/ui/debug.h
	${libmlk-ui_SOURCE_DIR}/ui/frame.c
	${libmlk-ui_SOURCE_DIR}/ui/frame.h
	${libmlk-ui_SOURCE_DIR}/ui/gridmenu.c
	${libmlk-ui_SOURCE_DIR}/ui/gridmenu.h
	${libmlk-ui_SOURCE_DIR}/ui/label.c
	${libmlk-ui_SOURCE_DIR}/ui/label.h
	${libmlk-ui_SOURCE_DIR}/ui/theme.c
	${libmlk-ui_SOURCE_DIR}/ui/theme.h
	${libmlk-ui_SOURCE_DIR}/ui/ui.c
	${libmlk-ui_SOURCE_DIR}/ui/ui.h
	${libmlk-ui_SOURCE_DIR}/ui/ui_p.h
)

molko_define_library(
	TARGET libmlk-ui
	TRANSLATIONS fr
	SOURCES ${SOURCES} ${PO} ${ASSETS}
	ASSETS ${ASSETS}
	LIBRARIES
		libmlk-core
	INCLUDES
		PUBLIC
			$<BUILD_INTERFACE:${libmlk-ui_SOURCE_DIR}>
)

source_group(TREE ${libmlk-ui_SOURCE_DIR} FILES ${ASSETS} ${PO} ${SOURCES})
