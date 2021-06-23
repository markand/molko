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

# Don't use absolute paths except for PREFIX.
PREFIX=                 /usr/local
BINDIR=                 bin
LIBDIR=                 lib
DATADIR=                share
LOCALEDIR=              share/locale

# Compile time options.
NLS=                    1

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

# Not a real target, only headers.
LIBMLK_DATA_ASTS=       libmlk-data/fonts/cubic.h \
                        libmlk-data/fonts/lato.h \
                        libmlk-data/fonts/pirata-one.h \
                        libmlk-data/fonts/teutonic.h \
                        libmlk-data/images/battle-background.h \
                        libmlk-data/images/black-cat.h \
                        libmlk-data/images/haunted-wood.h \
                        libmlk-data/music/vabsounds-romance.h \
                        libmlk-data/sounds/fire.h \
                        libmlk-data/sounds/potion.h \
                        libmlk-data/sprites/chest.h \
                        libmlk-data/sprites/explosion.h \
                        libmlk-data/sprites/faces.h \
                        libmlk-data/sprites/john-sword.h \
                        libmlk-data/sprites/john-walk.h \
                        libmlk-data/sprites/john.h \
                        libmlk-data/sprites/numbers.h \
                        libmlk-data/sprites/people.h \
                        libmlk-data/sprites/ui-cursor.h

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

TARGETS=                ${LIBMLK_ADVENTURE} \
                        ${LIBMLK_CORE} \
                        ${LIBMLK_RPG} \
                        ${LIBMLK_SQLITE} \
                        ${LIBMLK_UI} \
                        ${MLK_ADVENTURE} \
                        ${MLK_BCC} \
                        ${MLK_MAP} \
                        ${MLK_TILESET}

SDL2_INCS:=             $(shell pkg-config --cflags sdl2 SDL2_mixer SDL2_ttf SDL2_image)
SDL2_LIBS:=             $(shell pkg-config --libs sdl2 SDL2_mixer SDL2_ttf SDL2_image)

JANSSON_INCS:=          $(shell pkg-config --cflags jansson)
JANSSON_LIBS:=          $(shell pkg-config --libs jansson)

ZSTD_INCS:=             $(shell pkg-config --cflags libzstd)
ZSTD_LIBS:=             $(shell pkg-config --libs libzstd)

INCS=                   -I. \
                        -Iextern/libsqlite \
                        -Ilibmlk-core \
                        -Ilibmlk-ui \
                        -Ilibmlk-rpg \
                        -Ilibmlk-adventure \
                        ${SDL2_INCS}
DEFS=                   -DMOLKO_BINDIR=\"${BINDIR}\" \
                        -DMOLKO_DATADIR=\"${DATADIR}\" \
                        -DMOLKO_LOCALEDIR=\"${LOCALEDIR}\" \
                        -DMOLKO_ABS_BINDIR=\"${PREFIX}/${BINDIR}\" \
                        -DMOLKO_ABS_DATADIR=\"${PREFIX}/${DATADIR}\" \
                        -DMOLKO_ABS_LOCALEDIR=\"${PREFIX}/${LOCALEDIR}\"

ifeq (${NLS},1)
LIBS+=                  -lintl
SED.nls=                s/@define WITH_NLS@/\#define MOLKO_WITH_NLS/
else
SED.nls=                /@define WITH_NLS@/d
endif

.SUFFIXES:
.SUFFIXES: .c .o .h .map .ogg .png .sql .tileset .ttf

all: ${TARGETS}

config.h: config.h.in
	@echo "SED  $<"
	@sed -e "${SED.nls}" < $< > $@

.c.o:
	@echo "CC   $<"
	@${CC} -MMD ${INCS} ${DEFS} ${CFLAGS} -c $< -o $@

# Binary files.
.ttf.h .png.h .ogg.h:
	@echo "BCC  $<"
	@${MLK_BCC} -csu $< assets_$(notdir $<) > $@

# Text files.
.sql.h:
	@echo "BCC  $<"
	@${MLK_BCC} -0csu $< assets_$(notdir $<) > $@

# Maps
.json.map:
	@echo "MAP  $<"
	@${MLK_MAP} < $< > $@

.tileset.map:
	@echo "TS   $<"
	@${MLK_TILESET} < $< > $@

-include ${LIBMLK_ADVENTURE_DEPS}
-include ${LIBMLK_CORE_DEPS}
-include ${LIBMLK_RPG_DEPS}
-include ${LIBMLK_SQLITE_DEPS}
-include ${LIBMLK_UI_DEPS}
-include ${MLK_ADVENTURE_DEPS}
-include ${MLK_BCC_DEPS}
-include ${MLK_MAP_DEPS}
-include ${MLK_TILESET_DEPS}

${LIBMLK_SQLITE}: ${LIBMLK_SQLITE_OBJS}
	@echo "AR   $@"
	@${AR} -rc $@ ${LIBMLK_SQLITE_OBJS}

${LIBMLK_CORE_OBJS}: config.h

${LIBMLK_CORE}: ${LIBMLK_CORE_OBJS}
	@echo "AR   $@"
	@${AR} -rc $@ ${LIBMLK_CORE_OBJS}

${LIBMLK_UI_ASTS}: ${MLK_BCC}
${LIBMLK_UI_OBJS}: ${LIBMLK_UI_ASTS}

${LIBMLK_UI}: ${LIBMLK_CORE} ${LIBMLK_UI_OBJS}
	@echo "AR   $@"
	@${AR} -rc $@ ${LIBMLK_UI_OBJS}

${LIBMLK_RPG_ASTS}: ${MLK_BCC}
${LIBMLK_RPG_OBJS}: ${LIBMLK_RPG_ASTS}

${LIBMLK_RPG}: ${LIBMLK_UI} ${LIBMLK_SQLITE} ${LIBMLK_RPG_OBJS}
	@echo "AR   $@"
	@${AR} -rc $@ ${LIBMLK_RPG_OBJS}

${LIBMLK_ADVENTURE_ASTS}: ${MLK_BCC}
${LIBMLK_ADVENTURE_OBJS}: ${LIBMLK_ADVENTURE_ASTS}

${LIBMLK_ADVENTURE}: ${LIBMLK_RPG} ${LIBMLK_ADVENTURE_OBJS}
	@echo "AR   $@"
	@${AR} -rc $@ ${LIBMLK_ADVENTURE_OBJS}

${MLK_BCC}: ${MLK_BCC_OBJS}
	@echo "CCLD $@"
	@${CC} ${CFLAGS} -o $@ ${MLK_BCC_OBJS} ${SDL2_LIBS} ${LDFLAGS}

${MLK_TILESET}: ${MLK_TILESET_OBJS}
	@echo "CCLD $@"
	@${CC} ${CFLAGS} -o $@ ${MLK_TILESET_OBJS} ${JANSSON_LIBS} ${LDFLAGS}

${MLK_MAP}: ${MLK_MAP_OBJS}
	@echo "CCLD $@"
	@${CC} ${CFLAGS} -o $@ ${MLK_MAP_OBJS} ${JANSSON_LIBS} ${LDFLAGS}

${MLK_ADVENTURE_OBJS}: ${LIBMLK_ADVENTURE}

${MLK_ADVENTURE}: ${MLK_ADVENTURE_OBJS}
	@echo "CCLD $@"
	@${CC} ${CFLAGS} -o $@ ${MLK_ADVENTURE_OBJS} ${LIBMLK_ADVENTURE} \
		${LIBMLK_RPG} ${LIBMLK_UI} ${LIBMLK_CORE} ${LIBMLK_SQLITE} \
		${SDL2_LIBS} ${ZSTD_LIBS} ${LIBS} ${LDFLAGS}

clean:
	@echo "cleaning"
	@rm -f config.h tags
	@rm -f ${LIBMLK_ADVENTURE} ${LIBMLK_ADVENTURE_OBJS} ${LIBMLK_ADVENTURE_DEPS}
	@rm -f ${LIBMLK_CORE} ${LIBMLK_CORE_OBJS} ${LIBMLK_CORE_DEPS}
	@rm -f ${LIBMLK_RPG} ${LIBMLK_RPG_OBJS} ${LIBMLK_RPG_DEPS} ${LIBMLK_RPG_ASTS}
	@rm -f ${LIBMLK_UI} ${LIBMLK_UI_OBJS} ${LIBMLK_UI_DEPS} ${LIBMLK_UI_ASTS}
	@rm -f ${LIBMLK_SQLITE} ${LIBMLK_SQLITE_OBJS} ${LIBMLK_SQLITE_DEPS} ${LIBMLK_SQLITE_ASTS}
	@rm -f ${LIBMLK_DATA_ASTS}
	@rm -f ${MLK_ADVENTURE} ${MLK_ADVENTURE_OBJS} ${MLK_ADVENTURE_DEPS}
	@rm -f ${MLK_BCC} ${MLK_BCC_OBJS} ${MLK_BCC_DEPS}
	@rm -f ${MLK_MAP} ${MLK_MAP_OBJS} ${MLK_MAP_DEPS}
	@rm -f ${MLK_TILESET} ${MLK_TILESET_OBJS} ${MLK_TILESET_DEPS}

.PHONY: all clean
