#
# Makefile -- basic Makefile for Molko's Adventure
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

.POSIX:

CC=             clang
CFLAGS=         -O3 -DNDEBUG -D_XOPEN_SOURCE=700 -std=c99 -Wall -Wextra
CPPFLAGS=       -MMD
PROG=           molko
LIB=            libmolko.a
SRCS=           src/animation.c \
                src/clock.c \
                src/font.c \
                src/image.c \
                src/sprite.c \
                src/texture.c \
                src/window.c
TESTS=          tests/test-color.c
OBJS=           ${SRCS:.c=.o}
DEPS=           ${SRCS:.c=.d}

SDL_CFLAGS=     `pkg-config --cflags sdl2 SDL2_image SDL2_ttf`
SDL_LDFLAGS=    `pkg-config --libs sdl2 SDL2_image SDL2_ttf`

.SUFFIXES:
.SUFFIXES: .c .o

all: ${PROG}

-include ${DEPS}

.c.o:
	${CC} ${SDL_CFLAGS} ${CPPFLAGS} ${CFLAGS} -c $< -o $@

.c:
	${CC} -I extern/libgreatest -I src -o $@ ${CPPFLAGS} ${CFLAGS} $< ${LIB} ${SDL_LDFLAGS} ${LDFLAGS}

${LIB}: ${OBJS}
	${AR} -rcs $@ ${OBJS}

${PROG}: ${LIB} src/main.o
	${CC} -o $@ src/main.o ${LIB} ${SDL_LDFLAGS} ${LDFLAGS}

clean:
	rm -f molko ${OBJS} ${DEPS}

tests: ${TESTS:.c=}
	for t in $?; do ./$$t; done

.PHONY: clean
