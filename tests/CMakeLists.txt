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

project(tests)

molko_define_test(TARGET action SOURCES test-action.c)
molko_define_test(TARGET action-script SOURCES test-action-script.c)
molko_define_test(TARGET alloc SOURCES test-alloc.c)
molko_define_test(TARGET color SOURCES test-color.c)
molko_define_test(TARGET error SOURCES test-error.c)

molko_define_test(
	TARGET map
	SOURCES test-map.c
	FLAGS DIRECTORY="${tests_SOURCE_DIR}/assets/maps/"
)

molko_define_test(TARGET save SOURCES test-save.c)
molko_define_test(TARGET state SOURCES test-state.c)
molko_define_test(TARGET drawable SOURCES test-drawable.c)

molko_define_test(
	TARGET tileset
	SOURCES test-tileset.c
	FLAGS DIRECTORY="${tests_SOURCE_DIR}/assets/maps/"
)
