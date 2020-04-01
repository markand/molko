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

#
# User options begin here.
# -------------------------------------------------------------------
#
# CC:           C compiler
# AR:           Archiver
# CFLAGS:       General C flags
# LDFLAGS:      General linker flags
#
# PREFIX:       Installation prefix
# BINDIR:       Where to install binaries
# SHAREDIR:     Where to install resources
#

CC=             cc
AR=             ar
CFLAGS=         -O0 -std=c11 -g

PREFIX=         /usr/local
BINDIR=         ${PREFIX}/bin
SHAREDIR=       ${PREFIX}/share

# End of user options.

SQLITE_SRC=     extern/libsqlite/sqlite3.c
SQLITE_OBJ=     extern/libsqlite/sqlite3.o
SQLITE_LIB=     extern/libsqlite/libsqlite3.a
SQLITE_FLAGS=   -DSQLITE_THREADSAFE=0 \
                -DSQLITE_OMIT_LOAD_EXTENSION \
                -DSQLITE_OMIT_DEPRECATED \
                -DSQLITE_DEFAULT_FOREIGN_KEYS=1

CORE_SRCS=      src/core/animation.c                    \
                src/core/button.c                       \
                src/core/clock.c                        \
                src/core/debug.c                        \
                src/core/error.c                        \
                src/core/event.c                        \
                src/core/font.c                         \
                src/core/frame.c                        \
                src/core/game.c                         \
                src/core/image.c                        \
                src/core/inhibit.c                      \
                src/core/inventory.c                    \
                src/core/inventory_dialog.c             \
                src/core/label.c                        \
                src/core/map.c                          \
                src/core/map_state.c                    \
                src/core/maths.c                        \
                src/core/message.c                      \
                src/core/painter.c                      \
                src/core/panic.c                        \
                src/core/save.c                         \
                src/core/script.c                       \
                src/core/sound.c                        \
                src/core/sprite.c                       \
                src/core/sys.c                          \
                src/core/texture.c                      \
                src/core/theme.c                        \
                src/core/util.c                         \
                src/core/walksprite.c                   \
                src/core/wait.c                         \
                src/core/window.c
CORE_OBJS=      ${CORE_SRCS:.c=.o}
CORE_DEPS=      ${CORE_SRCS:.c=.d}

ADV_SRCS=       src/adventure/main.c                    \
                src/adventure/panic_state.c             \
                src/adventure/splashscreen_state.c      \
                src/adventure/mainmenu_state.c
ADV_OBJS=       ${ADV_SRCS:.c=.o}
ADV_DEPS=       ${ADV_SRCS:.c=.d}

SDL_CFLAGS=     `pkg-config --cflags sdl2 SDL2_image SDL2_mixer SDL2_ttf`
SDL_LDFLAGS=    `pkg-config --libs sdl2 SDL2_image SDL2_mixer SDL2_ttf`

JANSSON_CFLAGS= `pkg-config --cflags jansson`
JANSSON_LDFLAGS=`pkg-config --libs jansson`

EXAMPLES=       examples/example-message.c              \
                examples/example-inventory.c            \
                examples/example-sound.c
EXAMPLES_PRGS=  ${EXAMPLES:.c=}
EXAMPLES_OBJS=  ${EXAMPLES:.c=.o}
EXAMPLES_DEPS=  ${EXAMPLES:.c=.d}

TESTS=          tests/test-color.c                      \
                tests/test-error.c                      \
                tests/test-inventory.c                  \
                tests/test-map.c                        \
                tests/test-panic.c                      \
                tests/test-save.c                       \
                tests/test-script.c
TESTS_INCS=     -I extern/libgreatest -I src/core ${SDL_CFLAGS}
TESTS_PRGS=     ${TESTS:.c=}
TESTS_OBJS=     ${TESTS:.c=.o}
TESTS_DEPS=     ${TESTS:.c=.d}

TOOLS=          tools/molko-map.c                       \
                tools/molko-bcc.c
TOOLS_PRGS=     ${TOOLS:.c=}
TOOLS_DEPS=     ${TOOLS:.c=.d}

MY_CFLAGS=      -D_XOPEN_SOURCE=700 \
                -DPREFIX=\""${PREFIX}"\" \
                -DBINDIR=\""${BINDIR}"\" \
                -DSHAREDIR=\""${SHAREDIR}"\" \
                -Iextern/libsqlite \
                -Iextern/libgreatest \
                -Isrc/core \
                -Isrc/adventure
MY_LDFLAGS=     -lm

.SUFFIXES:
.SUFFIXES: .o .c

all: molko

-include ${CORE_DEPS} ${ADV_DEPS} ${TESTS_DEPS} ${TOOLS_DEPS}

.c.o:
	${CC} ${MY_CFLAGS} ${SDL_CFLAGS} ${CFLAGS} -MMD -c $< -o $@

.c:
	${CC} ${MY_CFLAGS} -o $@ ${CFLAGS} $< libmolko.a ${SQLITE_LIB} ${SDL_LDFLAGS} ${MY_LDFLAGS} ${LDFLAGS}

.o:
	${CC} -o $@ $< libmolko.a ${SQLITE_LIB} ${SDL_LDFLAGS} ${MY_LDFLAGS} ${LDFLAGS}

# {{{ Core

${SQLITE_OBJ}: ${SQLITE_SRC}
	${CC} ${CFLAGS} ${SQLITE_FLAGS} -c ${SQLITE_SRC} -o $@

${SQLITE_LIB}: ${SQLITE_OBJ}
	${AR} -rc $@ ${SQLITE_OBJ}

libmolko.a: ${CORE_OBJS} ${SQLITE_LIB}
	${AR} -rc $@ ${CORE_OBJS}

# }}}

# {{{ Molko's Adventure

molko: libmolko.a ${ADV_OBJS}
	${CC} -o $@ ${ADV_OBJS} libmolko.a ${SQLITE_LIB} ${SDL_LDFLAGS} ${MY_LDFLAGS} ${LDFLAGS}

# }}}

# {{{ Examples

${EXAMPLES_OBJS}: libmolko.a

examples: ${EXAMPLES_PRGS}

# }}}

# {{{ Tests

${TESTS_OBJS}: libmolko.a

tests: ${TESTS_PRGS}
	for t in ${TESTS_PRGS}; do ./$$t; done

# }}}

# {{{ Tools

tools: ${TOOLS_PRGS}

# Custom rule: does not depend on anything else than jansson.
tools/molko-map: tools/molko-map.c
	${CC} ${MY_CFLAGS} -o $@ tools/molko-map.c ${CFLAGS} ${JANSSON_CFLAGS} ${JANSSON_LDFLAGS} ${LDFLAGS}

# Custom rule: does not depend on anything.
tools/molko-bcc: tools/molko-bcc.c
	${CC} ${MY_CFLAGS} -o $@ tools/molko-bcc.c ${LDFLAGS}

# }}}

# {{{ Docs

doxygen:
	doxygen doxygen/Doxyfile

# }}}

# {{{ Misc

everything: molko ${EXAMPLES_PRGS} ${TOOLS_PRGS} ${TESTS_PRGS}

install:
	mkdir -p ${DESTDIR}${BINDIR}
	cp molko ${DESTDIR}${BINDIR}
	chmod 755 ${DESTDIR}${BINDIR}/molko
	mkdir -p ${DESTDIR}${SHAREDIR}/molko
	cp -R assets/* ${DESTDIR}${SHAREDIR}/molko

clean-core:
	rm -f ${SQLITE_OBJ} ${SQLITE_LIB}
	rm -f libmolko.a
	rm -f ${CORE_OBJS} ${CORE_DEPS}

clean-doxygen:
	rm -rf doxygen/html doxygen/man

clean-examples:
	rm -f ${EXAMPLES_PRGS} ${EXAMPLES_OBJS} ${EXAMPLES_DEPS}

clean-molko:
	rm -f molko ${ADV_OBJS} ${ADV_DEPS}

clean-tests:
	rm -f ${TESTS_PRGS} ${TESTS_OBJS} ${TESTS_DEPS}

clean-tools:
	rm -f ${TOOLS_PRGS} ${TOOLS_DEPS}

clean: clean-core \
       clean-doxygen \
       clean-examples \
       clean-molko \
       clean-tests \
       clean-tools

# }}}

.PHONY: all \
        clean-core \
        clean-doxygen \
        clean-examples \
        clean-molko \
        clean-tests \
        clean-tools \
        doxygen \
        everything \
        examples \
        tests \
        tools
