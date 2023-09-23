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

project(libmlk-duktape)

mlk_library(
	NAME libmlk-duktape
	FOLDER extern
	INSTALL
	SOURCES
		${libmlk-duktape_SOURCE_DIR}/duk_config.h
		${libmlk-duktape_SOURCE_DIR}/duk_module.c
		${libmlk-duktape_SOURCE_DIR}/duk_module.h
		${libmlk-duktape_SOURCE_DIR}/duktape.c
		${libmlk-duktape_SOURCE_DIR}/duktape.h
	INCLUDES
		PUBLIC $<BUILD_INTERFACE:${libmlk-duktape_SOURCE_DIR}>
)
