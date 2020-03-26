#!/bin/sh
#
# package-mingw64.sh -- create fakeroot directory for MinGW-w64
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

verbose=0

die()
{
	echo "$1" 1>&2
	exit 1
}

info()
{
	if [ $verbose -eq 1 ]; then
		echo $1
	fi
}

depends()
{
	ldd molko.exe | grep -E "/mingw" | awk '{ print $3 }'
}

usage()
{
	echo "Create a directory suitable for packaging." 1>&2
	echo "" 1>&2
	echo "usage: $(basename $0) [-v] output-directory" 1>&2
	exit 1
}

if [ ! -f Makefile ]; then
	die "abort: must be ran from top directory"
fi

if [ ! -f molko.exe ]; then
	die "abort: no molko.exe binary found, did you build?"
fi

while getopts "v" opt; do
	case $opt in
	v)
		verbose=1
		;;
	*)
		usage
		;;
	esac
done

shift $((OPTIND - 1))

if [ $# -eq 0 ] || [ -z $1 ]; then
	usage
fi

output=${1:-package}

info "Creating package in $output"

rm -rf "$output"
mkdir -p "$output"

info "Copying molko.exe"
cp molko.exe "$output"

info "Copying assets"
cp -R assets "$output"

depends | while read -r file; do
	info "Copying dependency $file"
	cp -f "$file" "$output"
done

info "Molko's Adventure is ready in $output"
