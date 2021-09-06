#
# GNUmakefile -- GNU make for molko
#
# Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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

CC=                     cc
CFLAGS=                 -O0 -g
AR=                     ar

MSGFMT=                 msgfmt
MSGMERGE=               msgmerge
XGETTEXT=               xgettext

# Don't use absolute paths except for PREFIX.
PREFIX=                 /usr/local
BINDIR=                 bin
LIBDIR=                 lib
DATADIR=                share
LOCALEDIR=              share/locale

# Compile time options.
NLS=                    1
COMPRESS=               1
ZSTD=                   1

LIBMLK_SQLITE=          extern/libsqlite/libmlk-sqlite.a
LIBMLK_SQLITE_SRCS=     extern/libsqlite/sqlite3.c
LIBMLK_SQLITE_OBJS=     ${LIBMLK_SQLITE_SRCS:.c=.o}
LIBMLK_SQLITE_DEPS=     ${LIBMLK_SQLITE_SRCS:.c=.d}

LIBMLK_CORE=            libmlk-core/libmlk-core.a
LIBMLK_CORE_SRCS=       libmlk-core/core/action.c \
                        libmlk-core/core/alloc.c \
                        libmlk-core/core/animation.c \
                        libmlk-core/core/clock.c \
                        libmlk-core/core/core.c \
                        libmlk-core/core/drawable.c \
                        libmlk-core/core/error.c \
                        libmlk-core/core/event.c \
                        libmlk-core/core/font.c \
                        libmlk-core/core/game.c \
                        libmlk-core/core/image.c \
                        libmlk-core/core/maths.c \
                        libmlk-core/core/music.c \
                        libmlk-core/core/painter.c \
                        libmlk-core/core/panic.c \
                        libmlk-core/core/port.c \
                        libmlk-core/core/script.c \
                        libmlk-core/core/sound.c \
                        libmlk-core/core/sprite.c \
                        libmlk-core/core/state.c \
                        libmlk-core/core/sys.c \
                        libmlk-core/core/texture.c \
                        libmlk-core/core/trace.c \
                        libmlk-core/core/translate.c \
                        libmlk-core/core/util.c \
                        libmlk-core/core/window.c \
                        libmlk-core/core/zfile.c
LIBMLK_CORE_OBJS=       ${LIBMLK_CORE_SRCS:.c=.o}
LIBMLK_CORE_DEPS=       ${LIBMLK_CORE_SRCS:.c=.d}

ifeq (${NLS},1)
LIBMLK_CORE_PO=         libmlk-core/nls/fr.po
LIBMLK_CORE_MO=         ${LIBMLK_CORE_PO:.po=.mo}
endif

LIBMLK_UI=              libmlk-ui/libmlk-ui.a
LIBMLK_UI_SRCS=         libmlk-ui/ui/align.c \
                        libmlk-ui/ui/button.c \
                        libmlk-ui/ui/checkbox.c \
                        libmlk-ui/ui/debug.c \
                        libmlk-ui/ui/frame.c \
                        libmlk-ui/ui/gridmenu.c \
                        libmlk-ui/ui/label.c \
                        libmlk-ui/ui/theme.c \
                        libmlk-ui/ui/ui.c
LIBMLK_UI_OBJS=         ${LIBMLK_UI_SRCS:.c=.o}
LIBMLK_UI_DEPS=         ${LIBMLK_UI_SRCS:.c=.d}
LIBMLK_UI_ASTS=         libmlk-ui/assets/fonts/opensans-light.h \
                        libmlk-ui/assets/fonts/opensans-regular.h

ifeq (${NLS},1)
LIBMLK_UI_PO=           libmlk-ui/nls/fr.po
LIBMLK_UI_MO=           ${LIBMLK_UI_PO:.po=.mo}
endif

LIBMLK_RPG=             libmlk-rpg/libmlk-rpg.a
LIBMLK_RPG_SRCS=        libmlk-rpg/rpg/battle-bar.c \
                        libmlk-rpg/rpg/battle-entity-state-attacking.c \
                        libmlk-rpg/rpg/battle-entity-state-blinking.c \
                        libmlk-rpg/rpg/battle-entity-state-moving.c \
                        libmlk-rpg/rpg/battle-entity-state-normal.c \
                        libmlk-rpg/rpg/battle-entity-state.c \
                        libmlk-rpg/rpg/battle-entity.c \
                        libmlk-rpg/rpg/battle-indicator.c \
                        libmlk-rpg/rpg/battle-message.c \
                        libmlk-rpg/rpg/battle-state-ai.c \
                        libmlk-rpg/rpg/battle-state-attacking.c \
                        libmlk-rpg/rpg/battle-state-check.c \
                        libmlk-rpg/rpg/battle-state-closing.c \
                        libmlk-rpg/rpg/battle-state-item.c \
                        libmlk-rpg/rpg/battle-state-lost.c \
                        libmlk-rpg/rpg/battle-state-menu.c \
                        libmlk-rpg/rpg/battle-state-opening.c \
                        libmlk-rpg/rpg/battle-state-selection.c \
                        libmlk-rpg/rpg/battle-state-sub.c \
                        libmlk-rpg/rpg/battle-state-victory.c \
                        libmlk-rpg/rpg/battle-state.c \
                        libmlk-rpg/rpg/battle.c \
                        libmlk-rpg/rpg/character.c \
                        libmlk-rpg/rpg/equipment.c \
                        libmlk-rpg/rpg/inventory.c \
                        libmlk-rpg/rpg/item.c \
                        libmlk-rpg/rpg/map-file.c \
                        libmlk-rpg/rpg/map.c \
                        libmlk-rpg/rpg/message.c \
                        libmlk-rpg/rpg/rpg.c \
                        libmlk-rpg/rpg/save.c \
                        libmlk-rpg/rpg/selection.c \
                        libmlk-rpg/rpg/spell.c \
                        libmlk-rpg/rpg/team.c \
                        libmlk-rpg/rpg/tileset-file.c \
                        libmlk-rpg/rpg/tileset.c \
                        libmlk-rpg/rpg/walksprite.c
LIBMLK_RPG_OBJS=        ${LIBMLK_RPG_SRCS:.c=.o}
LIBMLK_RPG_DEPS=        ${LIBMLK_RPG_SRCS:.c=.d}
LIBMLK_RPG_ASTS=        libmlk-rpg/assets/sql/character-load.h \
                        libmlk-rpg/assets/sql/character-save.h \
                        libmlk-rpg/assets/sql/init.h \
                        libmlk-rpg/assets/sql/property-get.h \
                        libmlk-rpg/assets/sql/property-remove.h \
                        libmlk-rpg/assets/sql/property-set.h

ifeq (${NLS},1)
LIBMLK_RPG_PO=          libmlk-rpg/nls/fr.po
LIBMLK_RPG_MO=          ${LIBMLK_RPG_PO:.po=.mo}
endif

LIBMLK_ADVENTURE=       libmlk-adventure/libmlk-adventure.a
LIBMLK_ADVENTURE_SRCS=  libmlk-adventure/adventure/action/chest.c \
                        libmlk-adventure/adventure/action/spawner.c \
                        libmlk-adventure/adventure/action/teleport.c \
                        libmlk-adventure/adventure/assets.c \
                        libmlk-adventure/adventure/character/black-cat.c \
                        libmlk-adventure/adventure/character/neth.c \
                        libmlk-adventure/adventure/dialog/save.c \
                        libmlk-adventure/adventure/item/potion.c \
                        libmlk-adventure/adventure/mapscene/mapscene.c \
                        libmlk-adventure/adventure/molko.c \
                        libmlk-adventure/adventure/spell/fire-minor.c \
                        libmlk-adventure/adventure/state/battle.c \
                        libmlk-adventure/adventure/state/continue.c \
                        libmlk-adventure/adventure/state/mainmenu.c \
                        libmlk-adventure/adventure/state/map.c \
                        libmlk-adventure/adventure/state/panic.c \
                        libmlk-adventure/adventure/state/splashscreen.c \
                        libmlk-adventure/adventure/trace_hud.c
LIBMLK_ADVENTURE_OBJS=  ${LIBMLK_ADVENTURE_SRCS:.c=.o}
LIBMLK_ADVENTURE_DEPS=  ${LIBMLK_ADVENTURE_SRCS:.c=.d}

ifeq (${NLS},1)
LIBMLK_ADVENTURE_PO=    libmlk-adventure/nls/fr.po
LIBMLK_ADVENTURE_MO=    ${LIBMLK_ADVENTURE_PO:.po=.mo}
endif

# Not a real target, only generated maps and tilesets.
LIBMLK_DATA_ASTS=       libmlk-data/maps/map-world.map \
                        libmlk-data/maps/tileset-world.tileset

MLK_BCC=                tools/bcc/mlk-bcc
MLK_BCC_SRCS=           tools/bcc/main.c
MLK_BCC_OBJS=           ${MLK_BCC_SRCS:.c=.o}
MLK_BCC_DEPS=           ${MLK_BCC_SRCS:.c=.d}

MLK_TILESET=            tools/tileset/mlk-tileset
MLK_TILESET_SRCS=       tools/tileset/main.c
MLK_TILESET_OBJS=       ${MLK_TILESET_SRCS:.c=.o}
MLK_TILESET_DEPS=       ${MLK_TILESET_SRCS:.c=.d}

MLK_MAP=                tools/map/mlk-map
MLK_MAP_SRCS=           tools/map/main.c
MLK_MAP_OBJS=           ${MLK_MAP_SRCS:.c=.o}
MLK_MAP_DEPS=           ${MLK_MAP_SRCS:.c=.d}

MLK_ADVENTURE=          mlk-adventure/mlk-adventure
MLK_ADVENTURE_SRCS=     mlk-adventure/main.c
MLK_ADVENTURE_OBJS=     ${MLK_ADVENTURE_SRCS:.c=.o}
MLK_ADVENTURE_DEPS=     ${MLK_ADVENTURE_SRCS:.c=.d}

EXAMPLES=               examples/example-action/main \
                        examples/example-animation/main \
                        examples/example-audio/main \
                        examples/example-cursor/main \
                        examples/example-debug/main \
                        examples/example-drawable/main \
                        examples/example-font/main \
                        examples/example-gridmenu/main \
                        examples/example-label/main \
                        examples/example-message/main \
                        examples/example-sprite/main \
                        examples/example-trace/main \
                        examples/example-ui/main

TESTS=                  tests/test-action-script \
                        tests/test-action \
                        tests/test-alloc \
                        tests/test-character \
                        tests/test-color \
                        tests/test-drawable \
                        tests/test-error \
                        tests/test-map \
                        tests/test-save \
                        tests/test-state \
                        tests/test-tileset \
                        tests/test-util

LIBRARIES=              ${LIBMLK_ADVENTURE} \
                        ${LIBMLK_CORE} \
                        ${LIBMLK_RPG} \
                        ${LIBMLK_SQLITE} \
                        ${LIBMLK_UI}

TOOLS=                  ${MLK_BCC} \
                        ${MLK_MAP} \
                        ${MLK_TILESET}

PROGS=                  ${MLK_ADVENTURE}

TARGETS:=               ${LIBRARIES} ${PROGS}

ifeq (${NLS},1)
POT:=                   libmlk-core-pot \
                        libmlk-ui-pot \
                        libmlk-rpg-pot \
                        libmlk-adventure-pot
endif

SDL2_INCS:=             $(shell pkg-config --cflags sdl2 SDL2_mixer SDL2_ttf SDL2_image)
SDL2_LIBS:=             $(shell pkg-config --libs sdl2 SDL2_mixer SDL2_ttf SDL2_image)

JANSSON_INCS:=          $(shell pkg-config --cflags jansson)
JANSSON_LIBS:=          $(shell pkg-config --libs jansson)

ifeq (${ZSTD},1)
ZSTD_INCS:=             $(shell pkg-config --cflags libzstd)
ZSTD_LIBS:=             $(shell pkg-config --libs libzstd)
endif

INCS=                   -I. \
                        -Iextern/libsqlite \
                        -Iextern/libgreatest \
                        -Ilibmlk-core \
                        -Ilibmlk-ui \
                        -Ilibmlk-rpg \
                        -Ilibmlk-adventure
DEFS=                   -DMOLKO_PREFIX=\"${PREFIX}\" \
                        -DMOLKO_BINDIR=\"${BINDIR}\" \
                        -DMOLKO_DATADIR=\"${DATADIR}\" \
                        -DMOLKO_LOCALEDIR=\"${LOCALEDIR}\"

ifeq (${NLS},1)
SED.nls=                s/@define WITH_NLS@/\#define MOLKO_WITH_NLS/
else
SED.nls=                /@define WITH_NLS@/d
endif

ifeq (${ZSTD},1)
SED.zstd=               s/@define WITH_ZSTD@/\#define MOLKO_WITH_ZSTD/
else
SED.zstd=               /@define WITH_ZSTD@/d
endif

# Can't use standard input otherwise frame content size isn't available.
ifeq (${COMPRESS},1)
TILESET.cmd=            ${MLK_TILESET} < $< > $@.tmp && zstd -17 $@.tmp --rm -fqo $@
MAP.cmd=                ${MLK_MAP} < $< > $@.tmp && zstd -17 $@.tmp --rm -fqo $@
else
TILESET.cmd=            ${MLK_TILESET} < $< > $@
MAP.cmd=                ${MLK_MAP} < $< > $@
endif

CMD.ar=                 ${AR} -rc $@ $^
CMD.cc=                 ${CC} ${DEFS} ${INCS} ${CFLAGS} -MMD -c $< -o $@ ${LDFLAGS}
CMD.ccld=               ${CC} ${DEFS} ${INCS} ${CFLAGS} -o $@ $^ ${LIBS} ${LDFLAGS}

# All libraries required when linking to Molko's Adventure API.
LIBMLK_ALL:=            ${LIBMLK_ADVENTURE}
LIBMLK_ALL+=            ${LIBMLK_RPG}
LIBMLK_ALL+=            ${LIBMLK_UI}
LIBMLK_ALL+=            ${LIBMLK_CORE}
LIBMLK_ALL+=            ${LIBMLK_SQLITE}
LIBMLK_ALL+=            ${SDL2_LIBS}
LIBMLK_ALL+=            ${ZSTD_LIBS}

ifeq (${NLS},1)
LIBMLK_ALL+=            -lintl
endif

.SUFFIXES:
.SUFFIXES: .c .o .h .json .map .ogg .png .sql .tileset .ttf .mo .po

all: ${TARGETS}

config.h: config.h.in
	sed -e "${SED.nls}" \
		-e "${SED.zstd}" < $< > $@

.c.o:
	${CMD.cc}

.po.mo:
	${MSGFMT} -o $@ $<

# Binary files.
.ttf.h .png.h .ogg.h:
	${MLK_BCC} -csu $< assets_$(notdir $<) > $@

# Text files.
.sql.h:
	${MLK_BCC} -0csu $< assets_$(notdir $<) > $@

# Maps.
.json.map:
	${MAP.cmd}

# Tilesets.
.json.tileset:
	${TILESET.cmd}

-include ${LIBMLK_ADVENTURE_DEPS}
-include ${LIBMLK_CORE_DEPS}
-include ${LIBMLK_RPG_DEPS}
-include ${LIBMLK_SQLITE_DEPS}
-include ${LIBMLK_UI_DEPS}
-include ${MLK_ADVENTURE_DEPS}
-include ${MLK_BCC_DEPS}
-include ${MLK_MAP_DEPS}
-include ${MLK_TILESET_DEPS}

#
# pot(pot, sources, po)
# ---------------------
#
# Regenerate .pot and update .po files.
#
define pot
	${XGETTEXT} -cj -k_ -kN_ -LC -s -o ${1} ${2}
	for po in ${3}; do \
		${MSGMERGE} --backup=off -qU $$po ${1}; \
	done
endef

#
# pot-install(name, mo, destdir)
#
define pot-install
	for mo in ${2}; do \
		mkdir -p ${3}${PREFIX}/${LOCALEDIR}/$$(basename $${mo%.mo})/LC_MESSAGES; \
		cp $$mo ${3}${PREFIX}/${LOCALEDIR}/$$(basename $${mo%.mo})/LC_MESSAGES/${1}.mo; \
	done
endef

# {{{ libmlk-sqlite

${LIBMLK_SQLITE}: ${LIBMLK_SQLITE_OBJS}
	${CMD.ar}

# }}}

# {{{ libmlk-core

${LIBMLK_CORE_OBJS}: config.h

${LIBMLK_CORE}: INCS += ${SDL2_INCS}
${LIBMLK_CORE}: ${LIBMLK_CORE_OBJS} ${LIBMLK_CORE_MO}
	${CMD.ar}

libmlk-core-pot:
	$(call pot, libmlk-core/nls/libmlk-core.pot, ${LIBMLK_CORE_SRCS}, ${LIBMLK_CORE_PO})

# }}}

# {{{ libmlk-ui

${LIBMLK_UI_ASTS}: ${MLK_BCC}
${LIBMLK_UI_OBJS}: ${LIBMLK_UI_ASTS} config.h

${LIBMLK_UI}: ${LIBMLK_UI_OBJS} ${LIBMLK_UI_MO}
	${CMD.ar}

libmlk-ui-pot:
	$(call pot, libmlk-ui/nls/libmlk-ui.pot, ${LIBMLK_UI_SRCS}, ${LIBMLK_UI_PO})

# }}}

# {{{ libmlk-rpg

${LIBMLK_RPG_ASTS}: ${MLK_BCC}
${LIBMLK_RPG_OBJS}: ${LIBMLK_RPG_ASTS} config.h

${LIBMLK_RPG}: ${LIBMLK_RPG_OBJS} ${LIBMLK_RPG_MO}
	${CMD.ar}

libmlk-rpg-pot:
	$(call pot, libmlk-rpg/nls/libmlk-rpg.pot, ${LIBMLK_RPG_SRCS}, ${LIBMLK_RPG_PO})

# }}}

# {{{ libmlk-adventure

${LIBMLK_ADVENTURE_ASTS}: ${MLK_BCC}
${LIBMLK_ADVENTURE_OBJS}: ${LIBMLK_ADVENTURE_ASTS} config.h

${LIBMLK_ADVENTURE}: ${LIBMLK_ADVENTURE_OBJS} ${LIBMLK_ADVENTURE_MO}
	${CMD.ar}

libmlk-adventure-pot:
	$(call pot, libmlk-adventure/nls/libmlk-adventure.pot, ${LIBMLK_ADVENTURE_SRCS}, ${LIBMLK_ADVENTURE_PO})

# }}}

# {{{ libmlk-data

${LIBMLK_DATA_ASTS}: ${MLK_MAP} ${MLK_TILESET}

# }}}

# {{{ tools

${MLK_BCC}: LIBS :=
${MLK_BCC}: ${MLK_BCC_OBJS}
	${CMD.ccld}

${MLK_TILESET}: LIBS := ${JANSSON_LIBS}
${MLK_TILESET}: ${MLK_TILESET_OBJS}
	${CMD.ccld}

${MLK_MAP}: LIBS := ${JANSSON_LIBS}
${MLK_MAP}: ${MLK_MAP_OBJS}
	${CMD.ccld}

# }}}

# {{{ mlk-adventure

${MLK_ADVENTURE_OBJS}: ${LIBRARIES} ${LIBMLK_DATA_ASTS}

${MLK_ADVENTURE}: LIBS := ${LIBMLK_ALL}
${MLK_ADVENTURE}: ${MLK_ADVENTURE_OBJS}
	${CMD.ccld}

# }}}

# {{{ examples

${EXAMPLES}: ${LIBRARIES}

examples/example-%/main: LIBS := ${LIBMLK_ALL}
examples/example-%/main: examples/example-%/main.c
	${CMD.ccld}

examples: ${EXAMPLES}

# }}}

# {{{ tests

${TESTS}: ${LIBMLK_ADVENTURE}

tests/test-%: CFLAGS += -DDIRECTORY=\"${CURDIR}/tests/assets\"
tests/test-%: LIBS := ${LIBMLK_ALL}
tests/test-%: tests/test-%.c
	${CMD.ccld}

tests: ${TESTS}
	for t in ${TESTS}; do printf "\n=> $$t <=\n" && $$t -v; done

# }}}

# {{{ clean

clean:
	rm -f config.h tags
	rm -f ${LIBMLK_ADVENTURE} ${LIBMLK_ADVENTURE_OBJS} ${LIBMLK_ADVENTURE_DEPS}
	rm -f ${LIBMLK_CORE} ${LIBMLK_CORE_OBJS} ${LIBMLK_CORE_DEPS}
	rm -f ${LIBMLK_RPG} ${LIBMLK_RPG_OBJS} ${LIBMLK_RPG_DEPS} ${LIBMLK_RPG_ASTS}
	rm -f ${LIBMLK_UI} ${LIBMLK_UI_OBJS} ${LIBMLK_UI_DEPS} ${LIBMLK_UI_ASTS}
	rm -f ${LIBMLK_SQLITE} ${LIBMLK_SQLITE_OBJS} ${LIBMLK_SQLITE_DEPS} ${LIBMLK_SQLITE_ASTS}
	rm -f ${LIBMLK_DATA_ASTS}
	rm -f ${MLK_ADVENTURE} ${MLK_ADVENTURE_OBJS} ${MLK_ADVENTURE_DEPS}
	rm -f ${MLK_BCC} ${MLK_BCC_OBJS} ${MLK_BCC_DEPS}
	rm -f ${MLK_MAP} ${MLK_MAP_OBJS} ${MLK_MAP_DEPS}
	rm -f ${MLK_TILESET} ${MLK_TILESET_OBJS} ${MLK_TILESET_DEPS}
	rm -f ${EXAMPLES} ${TESTS}

# }}}

# {{{ run

out: ${LIBMLK_DATA_ASTS} ${TARGETS}
	mkdir -p out
	${MAKE} DESTDIR=out install-data install
	touch out

run: out
	@./out/${PREFIX}/${BINDIR}/mlk-adventure

# }}}

install:
	mkdir -p ${DESTDIR}${PREFIX}/${BINDIR}
	cp ${TOOLS} ${TARGETS} ${DESTDIR}${PREFIX}/${BINDIR}
	mkdir -p ${DESTDIR}${PREFIX}/${LIBDIR}
	cp ${LIBRARIES} ${DESTDIR}${PREFIX}/${LIBDIR}

install-data:
	mkdir -p ${DESTDIR}${PREFIX}/${DATADIR}/mlk-adventure
	cp -R libmlk-data/fonts ${DESTDIR}${PREFIX}/${DATADIR}/mlk-adventure/fonts
	cp -R libmlk-data/images ${DESTDIR}${PREFIX}/${DATADIR}/mlk-adventure/images
	cp -R libmlk-data/music ${DESTDIR}${PREFIX}/${DATADIR}/mlk-adventure/music
	cp -R libmlk-data/sounds ${DESTDIR}${PREFIX}/${DATADIR}/mlk-adventure/sounds
	cp -R libmlk-data/sprites ${DESTDIR}${PREFIX}/${DATADIR}/mlk-adventure/sprites
	cp -R libmlk-data/maps ${DESTDIR}${PREFIX}/${DATADIR}/mlk-adventure/maps
ifeq (${NLS},1)
	$(call pot-install,libmlk-core,${LIBMLK_CORE_MO},${DESTDIR})
	$(call pot-install,libmlk-ui,${LIBMLK_UI_MO},${DESTDIR})
	$(call pot-install,libmlk-rpg,${LIBMLK_RPG_MO},${DESTDIR})
	$(call pot-install,libmlk-adventure,${LIBMLK_ADVENTURE_MO},${DESTDIR})
endif

# {{{ meta targets

tools: ${TOOLS}

everything: ${TARGETS} ${TESTS} ${EXAMPLES}

pot: ${POT}

# }}}

.PHONY: all clean examples everything install install-data pot run tests tools
