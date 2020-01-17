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

CC=             gcc
CFLAGS=         -MMD -O0 -DNDEBUG -std=c18 -Wall -Wextra -g
PROG=           molko
LIB=            libmolko.a
SRCS=           src/animation.c \
                src/clock.c \
                src/error.c \
                src/event.c \
                src/font.c \
                src/game.c \
                src/image.c \
                src/map.c \
                src/map_state.c \
                src/message.c \
                src/painter.c \
                src/sprite.c \
                src/sys.c \
                src/texture.c \
                src/util.c \
                src/splashscreen.c \
                src/walksprite.c \
                src/window.c
OBJS=           ${SRCS:.c=.o}
DEPS=           ${SRCS:.c=.d}

PREFIX=         /usr/local
BINDIR=         ${PREFIX}/bin
SHAREDIR=       ${PREFIX}/share

SDL_CFLAGS=     `pkg-config --cflags sdl2 SDL2_image SDL2_mixer SDL2_ttf`
SDL_LDFLAGS=    `pkg-config --libs sdl2 SDL2_image SDL2_mixer SDL2_ttf`

JANSSON_CFLAGS= `pkg-config --cflags jansson`
JANSSON_LDFLAGS=`pkg-config --libs jansson`

TESTS=          tests/test-color.c \
                tests/test-error.c \
                tests/test-map.c
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
	${CC} -DPREFIX=\""${PREFIX}"\" -DBINDIR=\""${BINDIR}"\" -DSHAREDIR=\""${SHAREDIR}"\" ${SDL_CFLAGS} ${CFLAGS} -c $< -o $@

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
	${CC} -o $@ $< ${CFLAGS} ${JANSSON_CFLAGS} ${JANSSON_LDFLAGS}

doxygen:
	doxygen doxygen/Doxyfile

install:
	mkdir -p ${DESTDIR}${BINDIR}
	cp molko ${DESTDIR}${BINDIR}
	chmod 755 ${DESTDIR}${BINDIR}/molko
	mkdir -p ${DESTDIR}${SHAREDIR}/molko
	cp -R assets/* ${DESTDIR}${SHAREDIR}/molko

clean:
	rm -f ${PROG} src/main.o src/main.d
	rm -rf doxygen/html doxygen/man
	rm -f ${LIB} ${OBJS} ${DEPS}
	rm -f ${TESTS_OBJS} ${TESTS_DEPS}
	rm -f ${TOOLS_OBJS} ${TOOLS_DEPS}

.PHONY: clean doxygen tests tools
