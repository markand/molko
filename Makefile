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
CFLAGS=         -MMD -O0 -std=c11 -Wall -Wextra -g -Wall -Wextra
# Use this instead to build a release.
# CFLAGS=         -MMD -O3 -DNDEBUG -std=c11 -Wall -Wextra
PROG=           molko
LIB=            libmolko.a

CORE_SRCS=      src/core/animation.c \
                src/core/clock.c \
                src/core/debug.c \
                src/core/error.c \
                src/core/event.c \
                src/core/font.c \
                src/core/game.c \
                src/core/image.c \
                src/core/inhibit.c \
                src/core/map.c \
                src/core/map_state.c \
                src/core/message.c \
                src/core/painter.c \
                src/core/script.c \
                src/core/sprite.c \
                src/core/sys.c \
                src/core/texture.c \
                src/core/util.c \
                src/core/walksprite.c \
                src/core/wait.c \
                src/core/window.c
CORE_OBJS=      ${CORE_SRCS:.c=.o}
CORE_DEPS=      ${CORE_SRCS:.c=.d}

ADV_SRCS=       src/adventure/main.c \
                src/adventure/splashscreen_state.c \
                src/adventure/mainmenu_state.c
ADV_OBJS=       ${ADV_SRCS:.c=.o}
ADV_DEPS=       ${ADV_SRCS:.c=.d}

PREFIX=         /usr/local
BINDIR=         ${PREFIX}/bin
SHAREDIR=       ${PREFIX}/share

SDL_CFLAGS=     `pkg-config --cflags sdl2 SDL2_image SDL2_mixer SDL2_ttf`
SDL_LDFLAGS=    `pkg-config --libs sdl2 SDL2_image SDL2_mixer SDL2_ttf`

JANSSON_CFLAGS= `pkg-config --cflags jansson`
JANSSON_LDFLAGS=`pkg-config --libs jansson`

TESTS=          tests/test-color.c \
                tests/test-error.c \
                tests/test-map.c \
                tests/test-script.c
TESTS_INCS=     -I extern/libgreatest -I src/core ${SDL_CFLAGS}
TESTS_LIBS=     ${LIB} ${SDL_LDFLAGS} ${LDFLAGS}
TESTS_OBJS=     ${TESTS:.c=}
TESTS_DEPS=     ${TESTS:.c=.d}

TOOLS=          tools/molko-map.c
TOOLS_OBJS=     ${TOOLS:.c=}
TOOLS_DEPS=     ${TOOLS:.c=.d}

DEFINES=        -DPREFIX=\""${PREFIX}"\" \
                -DBINDIR=\""${BINDIR}"\" \
                -DSHAREDIR=\""${SHAREDIR}"\"
INCLUDES=       -I src/core -I src/adventure

.SUFFIXES:
.SUFFIXES: .c .o

all: ${PROG}

-include ${CORE_DEPS} ${ADV_DEPS} ${TESTS_DEPS} ${TOOLS_DEPS}

.c.o:
	${CC} ${DEFINES} ${INCLUDES} ${SDL_CFLAGS} ${CFLAGS} -c $< -o $@

.c:
	${CC} ${TESTS_INCS} -o $@ ${CFLAGS} $< ${TESTS_LIBS}

${LIB}: ${CORE_OBJS}
	${AR} -rcs $@ ${CORE_OBJS}

${PROG}: ${LIB} ${ADV_OBJS}
	${CC} -o $@ ${ADV_OBJS} ${LIB} ${SDL_LDFLAGS} ${LDFLAGS}

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
	rm -rf doxygen/html doxygen/man
	rm -f ${LIB} ${PROG}
	rm -f ${CORE_OBJS} ${CORE_DEPS}
	rm -f ${ADV_OBJS} ${ADV_DEPS}
	rm -f ${TESTS_OBJS} ${TESTS_DEPS}
	rm -f ${TOOLS_OBJS} ${TOOLS_DEPS}

.PHONY: all clean doxygen tests tools
