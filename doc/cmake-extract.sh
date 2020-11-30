#!/bin/sh
#
# cmake-extract.sh -- extract documentation from CMake headers
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

prog=$(basename $0)

usage()
{
	printf "usage: $prog cmake-file\n" 1>&2
	exit 1
}

if [ $# -ne 1 ]; then
	usage
fi

#
# Remove ISC license header then every grep every lines that start with a #
# and finally remove them.
#

sed -e "1,19d" "$1" | grep '^#' | sed -e "s/^# *//"
