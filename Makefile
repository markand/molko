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
CFLAGS=         -MMD -O3 -DNDEBUG -std=c18 -Wall -Wextra
PROG=           molko
LIB=            libmolko.a
SRCS=           src/animation.c \
                src/clock.c \
                src/event.c \
                src/font.c \
                src/image.c \
                src/sprite.c \
                src/texture.c \
                src/window.c
OBJS=           ${SRCS:.c=.o}
DEPS=           ${SRCS:.c=.d}

SDL_CFLAGS=     `pkg-config --cflags sdl2 SDL2_image SDL2_ttf`
SDL_LDFLAGS=    `pkg-config --libs sdl2 SDL2_image SDL2_ttf`

EXPAT_CFLAGS=   `pkg-config --cflags expat`
EXPAT_LDFLAGS=  `pkg-config --libs expat`

TESTS=          tests/test-color.c
TESTS_INCS=     -I extern/libgreatest -I src ${SDL_CFLAGS}
TESTS_LIBS=     ${LIB} ${SDL_LDFLAGS} ${LDFLAGS}
TESTS_OBJS=     ${TESTS:.c=}
TESTS_DEPS=     ${TESTS:.c=.d}

TOOLS=          tools/molko-map.c
TOOLS_OBJS=     ${TOOLS:.c=}
TOOLS_DEPS=     ${TOOLS:.c=.d}

.SUFFIXES:
.SUFFIXES: .c .o

all: ${PROG}

-include ${DEPS} ${TESTS_DEPS} ${TOOLS_DEPS}

.c.o:
	${CC} ${SDL_CFLAGS} ${CFLAGS} -c $< -o $@

.c:
	${CC} ${TESTS_INCS} -o $@ ${CFLAGS} $< ${TESTS_LIBS}

${LIB}: ${OBJS}
	${AR} -rcs $@ ${OBJS}

${PROG}: ${LIB} src/main.o
	${CC} -o $@ src/main.o ${LIB} ${SDL_LDFLAGS} ${LDFLAGS}

${TESTS_OBJS}: ${LIB}

tests: ${TESTS_OBJS}
	for t in $?; do ./$$t; done

tools: ${TOOLS_OBJS}

tools/molko-map: tools/molko-map.c
	${CC} -o $@ $< ${CFLAGS} ${EXPAT_CFLAGS} ${EXPAT_LDFLAGS}

clean:
	rm -f ${PROG} src/main.o src/main.d
	rm -f ${LIB} ${OBJS} ${DEPS}
	rm -f ${TESTS_OBJS} ${TESTS_DEPS}
	rm -f ${TOOLS_OBJS} ${TOOLS_DEPS}

.PHONY: clean tests tools