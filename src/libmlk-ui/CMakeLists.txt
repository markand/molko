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

project(libmlk-ui)

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
	${libmlk-ui_SOURCE_DIR}/ui/notify.c
	${libmlk-ui_SOURCE_DIR}/ui/notify.h
	${libmlk-ui_SOURCE_DIR}/ui/theme.c
	${libmlk-ui_SOURCE_DIR}/ui/theme.h
	${libmlk-ui_SOURCE_DIR}/ui/ui.c
	${libmlk-ui_SOURCE_DIR}/ui/ui.h
	${libmlk-ui_SOURCE_DIR}/ui/ui_p.h
)

set(
	ASSETS
	${libmlk-ui_SOURCE_DIR}/assets/fonts/opensans-light.ttf
	${libmlk-ui_SOURCE_DIR}/assets/fonts/opensans-medium.ttf
	${libmlk-ui_SOURCE_DIR}/assets/fonts/opensans-regular.ttf
)

set(
	NLS
	${libmlk-ui_SOURCE_DIR}/nls/fr.po
)

mlk_library(
	NAME libmlk-ui
	SOURCES ${SOURCES} ${ASSETS} ${NLS}
	LANGS ${NLS}
	ASSETS ${ASSETS}
	LIBRARIES libmlk-core
	INCLUDES
		PRIVATE $<BUILD_INTERFACE:${libmlk-ui_BINARY_DIR}>
		PUBLIC $<BUILD_INTERFACE:${libmlk-ui_SOURCE_DIR}>
)

source_group(TREE ${libmlk-ui_SOURCE_DIR} FILES ${SOURCES} ${ASSETS} ${NLS})
