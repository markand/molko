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

project(doc)

set(
	SOURCES
	${doc_SOURCE_DIR}/CMakeLists.txt
	${doc_SOURCE_DIR}/mkdocs.yml
	${doc_SOURCE_DIR}/docs
	${doc_SOURCE_DIR}/docs/tools
	${doc_SOURCE_DIR}/docs/tools/map.md
	${doc_SOURCE_DIR}/docs/tools/bcc.md
	${doc_SOURCE_DIR}/docs/tools/tileset.md
	${doc_SOURCE_DIR}/docs/specs
	${doc_SOURCE_DIR}/docs/specs/map.md
	${doc_SOURCE_DIR}/docs/specs/tileset.md
	${doc_SOURCE_DIR}/docs/install.md
	${doc_SOURCE_DIR}/docs/index.md
	${doc_SOURCE_DIR}/docs/dev
	${doc_SOURCE_DIR}/docs/dev/faq.md
	${doc_SOURCE_DIR}/docs/dev/error.md
	${doc_SOURCE_DIR}/docs/dev/ownership.md
	${doc_SOURCE_DIR}/docs/dev/api
	${doc_SOURCE_DIR}/docs/dev/api/core
	${doc_SOURCE_DIR}/docs/dev/api/core/texture.md
	${doc_SOURCE_DIR}/docs/dev/api/core/mouse.md
	${doc_SOURCE_DIR}/docs/dev/api/core/inhibit.md
	${doc_SOURCE_DIR}/docs/dev/api/core/sprite.md
	${doc_SOURCE_DIR}/docs/dev/api/core/animation.md
	${doc_SOURCE_DIR}/docs/dev/api/core/music.md
	${doc_SOURCE_DIR}/docs/dev/api/core/image.md
	${doc_SOURCE_DIR}/docs/dev/api/core/zfile.md
	${doc_SOURCE_DIR}/docs/dev/api/core/alloc.md
	${doc_SOURCE_DIR}/docs/dev/api/core/sys.md
	${doc_SOURCE_DIR}/docs/dev/api/core/state.md
	${doc_SOURCE_DIR}/docs/dev/api/core/maths.md
	${doc_SOURCE_DIR}/docs/dev/api/core/script.md
	${doc_SOURCE_DIR}/docs/dev/api/core/error.md
	${doc_SOURCE_DIR}/docs/dev/api/core/color.md
	${doc_SOURCE_DIR}/docs/dev/api/core/window.md
	${doc_SOURCE_DIR}/docs/dev/api/core/sound.md
	${doc_SOURCE_DIR}/docs/dev/api/core/save.md
	${doc_SOURCE_DIR}/docs/dev/api/core/translate.md
	${doc_SOURCE_DIR}/docs/dev/api/core/event.md
	${doc_SOURCE_DIR}/docs/dev/api/core/trace.md
	${doc_SOURCE_DIR}/docs/dev/api/core/core.md
	${doc_SOURCE_DIR}/docs/dev/api/core/action.md
	${doc_SOURCE_DIR}/docs/dev/api/core/font.md
	${doc_SOURCE_DIR}/docs/dev/api/core/panic.md
	${doc_SOURCE_DIR}/docs/dev/api/core/key.md
	${doc_SOURCE_DIR}/docs/dev/api/core/painter.md
	${doc_SOURCE_DIR}/docs/dev/api/core/game.md
	${doc_SOURCE_DIR}/docs/dev/api/core/util.md
	${doc_SOURCE_DIR}/docs/dev/api/core/clock.md
	${doc_SOURCE_DIR}/docs/dev/api/core/drawable.md
	${doc_SOURCE_DIR}/docs/dev/howto
	${doc_SOURCE_DIR}/docs/dev/howto/01-init.md
	${doc_SOURCE_DIR}/docs/about.md
)

add_custom_target(
	doc
	SOURCES ${SOURCES}
)

add_custom_target(
	doc-serve
	SOURCES ${SOURCES}
	COMMAND mkdocs serve
	WORKING_DIRECTORY ${doc_SOURCE_DIR}
)

source_group(TREE ${doc_SOURCE_DIR} FILES ${SOURCES})
