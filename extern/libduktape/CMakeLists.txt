#
# CMakeLists.txt -- CMake build system for duktape
#
# Copyright (c) 2016-2019 David Demelier <markand@malikania.fr>
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

project(libduktape)

molko_define_library(
	TARGET libduktape
	FOLDER extern
	SOURCES
		${libduktape_SOURCE_DIR}/duk_config.h
		${libduktape_SOURCE_DIR}/duk_module.c
		${libduktape_SOURCE_DIR}/duk_module.h
		${libduktape_SOURCE_DIR}/duktape.c
		${libduktape_SOURCE_DIR}/duktape.h
	PUBLIC_INCLUDES
		$<BUILD_INTERFACE:${libduktape_SOURCE_DIR}>
)
