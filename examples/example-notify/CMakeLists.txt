#
# CMakeLists.txt -- CMake build system for Molko's Engine
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

project(example-notify)

set(
	SOURCES
	${example-notify_SOURCE_DIR}/main.c
)

set(
	ASSETS
	${example-notify_SOURCE_DIR}/assets/images/sword.png
)

mlk_executable(
	NAME example-notify
	FOLDER examples
	LIBRARIES libmlk-rpg
	ASSETS ${ASSETS}
	SOURCES ${ASSETS} ${SOURCES}
)

source_group(TREE ${example-notify_SOURCE_DIR} FILES ${ASSETS} ${SOURCES})
