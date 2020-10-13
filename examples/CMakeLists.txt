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

project(examples)

molko_define_executable(
	TARGET example-action
	SOURCES example-action.c
	FOLDER examples
	ASSETS
		${examples_SOURCE_DIR}/assets/sprites/chest.png
		${examples_SOURCE_DIR}/assets/sprites/people.png
	LIBRARIES libcore
)

molko_define_executable(
	TARGET example-inventory
	SOURCES example-inventory
	ASSETS
		${examples_SOURCE_DIR}/assets/images/fish.png
		${examples_SOURCE_DIR}/assets/images/potion.png
		${examples_SOURCE_DIR}/assets/images/sword.png
	FOLDER examples
	LIBRARIES libcore
)

molko_define_executable(
	TARGET example-message
	SOURCES example-message.c
	FOLDER examples
	LIBRARIES libcore
)

molko_define_executable(
	TARGET example-sound
	SOURCES example-sound.c
	FOLDER examples
	ASSETS
		${examples_SOURCE_DIR}/assets/sounds/vabsounds-romance.ogg 
	LIBRARIES libcore
)

molko_define_executable(
	TARGET example-drawable
	SOURCES example-drawable.c
	FOLDER examples
	LIBRARIES libcore
	ASSETS
		${examples_SOURCE_DIR}/assets/sprites/explosion.png
)
