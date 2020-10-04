#
# CMakeLists.txt -- CMake build system for libcore
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

project(libcore)

set(
	ASSETS
	${libcore_SOURCE_DIR}/assets/maps/test.map
	${libcore_SOURCE_DIR}/assets/images/message.png
	${libcore_SOURCE_DIR}/assets/sprites/test-walk.png
	${libcore_SOURCE_DIR}/assets/fonts/DejaVuSansCondensed.ttf
	${libcore_SOURCE_DIR}/assets/fonts/DejaVuSans.ttf
	${libcore_SOURCE_DIR}/assets/fonts/Lato-Regular.ttf
	${libcore_SOURCE_DIR}/assets/fonts/ComicNeue-Regular.ttf
	${libcore_SOURCE_DIR}/assets/fonts/pirata-one.ttf
	${libcore_SOURCE_DIR}/assets/fonts/teutonic1.ttf
	${libcore_SOURCE_DIR}/assets/fonts/knights-quest.ttf
	${libcore_SOURCE_DIR}/assets/tilesets/test.png
)

set(
	SOURCES
	${libcore_SOURCE_DIR}/action.h
	${libcore_SOURCE_DIR}/animation.c
	${libcore_SOURCE_DIR}/animation.h
	${libcore_SOURCE_DIR}/button.c
	${libcore_SOURCE_DIR}/button.h
	${libcore_SOURCE_DIR}/checkbox.c
	${libcore_SOURCE_DIR}/checkbox.h
	${libcore_SOURCE_DIR}/clock.c
	${libcore_SOURCE_DIR}/clock.h
	${libcore_SOURCE_DIR}/color.h
	${libcore_SOURCE_DIR}/debug.c
	${libcore_SOURCE_DIR}/debug.h
	${libcore_SOURCE_DIR}/error.c
	${libcore_SOURCE_DIR}/error.h
	${libcore_SOURCE_DIR}/error_p.h
	${libcore_SOURCE_DIR}/event.c
	${libcore_SOURCE_DIR}/event.h
	${libcore_SOURCE_DIR}/font.c
	${libcore_SOURCE_DIR}/font.h
	${libcore_SOURCE_DIR}/frame.c
	${libcore_SOURCE_DIR}/frame.h
	${libcore_SOURCE_DIR}/game.c
	${libcore_SOURCE_DIR}/game.h
	${libcore_SOURCE_DIR}/image.c
	${libcore_SOURCE_DIR}/image.h
	${libcore_SOURCE_DIR}/inhibit.c
	${libcore_SOURCE_DIR}/inhibit.h
	${libcore_SOURCE_DIR}/inventory.c
	${libcore_SOURCE_DIR}/inventory.h
	${libcore_SOURCE_DIR}/inventory_dialog.c
	${libcore_SOURCE_DIR}/inventory_dialog.h
	${libcore_SOURCE_DIR}/item.h
	${libcore_SOURCE_DIR}/key.h
	${libcore_SOURCE_DIR}/label.c
	${libcore_SOURCE_DIR}/label.h
	${libcore_SOURCE_DIR}/map.c
	${libcore_SOURCE_DIR}/map.h
	${libcore_SOURCE_DIR}/map_state.c
	${libcore_SOURCE_DIR}/map_state.h
	${libcore_SOURCE_DIR}/maths.c
	${libcore_SOURCE_DIR}/maths.h
	${libcore_SOURCE_DIR}/message.c
	${libcore_SOURCE_DIR}/message.h
	${libcore_SOURCE_DIR}/mouse.h
	${libcore_SOURCE_DIR}/painter.c
	${libcore_SOURCE_DIR}/painter.h
	${libcore_SOURCE_DIR}/panic.c
	${libcore_SOURCE_DIR}/panic.h
	${libcore_SOURCE_DIR}/plat.h
	${libcore_SOURCE_DIR}/save.c
	${libcore_SOURCE_DIR}/save.h
	${libcore_SOURCE_DIR}/script.c
	${libcore_SOURCE_DIR}/script.h
	${libcore_SOURCE_DIR}/sound.c
	${libcore_SOURCE_DIR}/sound.h
	${libcore_SOURCE_DIR}/sprite.c
	${libcore_SOURCE_DIR}/sprite.h
	${libcore_SOURCE_DIR}/state.h
	${libcore_SOURCE_DIR}/sys.c
	${libcore_SOURCE_DIR}/sys.h
	${libcore_SOURCE_DIR}/texture.c
	${libcore_SOURCE_DIR}/texture.h
	${libcore_SOURCE_DIR}/texture_p.h
	${libcore_SOURCE_DIR}/theme.c
	${libcore_SOURCE_DIR}/theme.h
	${libcore_SOURCE_DIR}/util.c
	${libcore_SOURCE_DIR}/util.h
	${libcore_SOURCE_DIR}/wait.c
	${libcore_SOURCE_DIR}/wait.h
	${libcore_SOURCE_DIR}/walksprite.c
	${libcore_SOURCE_DIR}/walksprite.h
	${libcore_SOURCE_DIR}/window.c
	${libcore_SOURCE_DIR}/window.h
	${libcore_SOURCE_DIR}/window_p.h
)

molko_define_library(
	TARGET libcore
	SOURCES ${SOURCES}
	ASSETS ${ASSETS}
	LIBRARIES
		SDL2::SDL2
		SDL2::image
		SDL2::mixer
		SDL2::ttf
	PRIVATE_FLAGS
		BINDIR="${CMAKE_INSTALL_BINDIR}"
		PREFIX="${CMAKE_INSTALL_PREFIX}"
		SHAREDIR="${CMAKE_INSTALL_DATADIR}"
	PUBLIC_FLAGS
		_XOPEN_SOURCE=700
	PUBLIC_INCLUDES
		$<BUILD_INTERFACE:${libcore_SOURCE_DIR}>
)
