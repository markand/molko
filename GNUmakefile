-include config.mk

# Toolchain options.
CC ?=                   clang
CFLAGS ?=               -g -O0

# Installation directory.
PREFIX ?=               /usr/local
LIBDIR ?=               $(PREFIX)/lib
INCDIR ?=               $(PREFIX)/include
MANDIR ?=               $(PREFIX)/share/man

# User options.
WITH_DEBUG ?=           no

OS :=                   $(shell uname -s)

SDL2_INCS ?=            $(shell pkg-config --cflags sdl2)
SDL2_LIBS ?=            $(shell pkg-config --libs sdl2)

SDL2_IMAGE_INCS ?=      $(shell pkg-config --cflags SDL2_image)
SDL2_IMAGE_LIBS ?=      $(shell pkg-config --libs SDL2_image)

SDL2_TTF_INCS ?=        $(shell pkg-config --cflags SDL2_ttf)
SDL2_TTF_LIBS ?=        $(shell pkg-config --libs SDL2_ttf)

SNDFILE_INCS ?=         $(shell pkg-config --cflags sndfile)
SNDFILE_LIBS ?=         $(shell pkg-config --libs sndfile)

JANSSON_INCS ?=         $(shell pkg-config --cflags jansson)
JANSSON_LIBS ?=         $(shell pkg-config --libs jansson)

ifeq ($(OS),Darwin)
OPENAL_LIBS ?=          -framework OpenAL
else
OPENAL_INCS ?=          $(shell pkg-config --cflags openal)
OPENAL_LIBS ?=          $(shell pkg-config --libs openal)
endif

MATH_LIBS ?=            -lm

# Global INCS, OPTS and DEFS for every targets.
INCS :=                 -Iextern/libdt \
                        -Iextern/libsqlite \
                        -Ilibmlk-util \
                        -Ilibmlk-core \
                        -Ilibmlk-ui \
                        -Ilibmlk-rpg \
                        -I. \
                        $(SDL2_INCS) \
                        $(SDL2_IMAGE_INCS) \
                        $(SDL2_TTF_INCS) \
                        $(OPENAL_INCS) \
                        $(SNDFILE_INCS)

OPTS +=                 -Wall -Wextra -pipe

ifeq ($(OS),Darwin)
OPTS +=                 -Wno-deprecated-declarations
endif

ifeq ($(WITH_DEBUG),yes)
OPTS +=                 -g -O0
else
OPTS +=                 -DNDEBUG -O3
endif

BCC_OPTS :=             -cs
MD ?=                   -MD

# Meta variable that contains all libraries, used for executables and tests.
LIBMLK =                $(LIBMLK_RPG) \
                        $(LIBMLK_UI) \
                        $(LIBMLK_CORE) \
                        $(LIBMLK_UTIL) \
                        $(LIBMLK_SQLITE) \
                        $(MATH_LIBS) \
                        $(SDL2_LIBS) \
                        $(SDL2_IMAGE_LIBS) \
                        $(SDL2_TTF_LIBS) \
                        $(OPENAL_LIBS) \
                        $(SNDFILE_LIBS)

.DEFAULT_GOAL :=        all

.SUFFIXES:
.SUFFIXES: .c .h .o .ogg .png .sql .ttf .wav

.c.o:
	$(CC) $(OPTS) $(INCS) $(DEFS) $(CFLAGS) $(MD) -c $< -o $@

.c:
	$(CC) $(OPTS) $(INCS) $(DEFS) $(CFLAGS) $< -o $@ $(OBJS) $(LIBS) $(LDFLAGS)

.ogg.h .png.h .ttf.h .sql.h .wav.h:
	$(MLK_BCC) $(BCC_OPTS) $< assets_$(basename $(<F)) > $@

%.a:
	$(AR) -rc $@ $(OBJS)

# {{{ libmlk-util

LIBMLK_UTIL :=          libmlk-util.a
LIBMLK_UTIL_SRCS :=     libmlk-util/mlk/util/fmemopen.c \
                        libmlk-util/mlk/util/openbsd/basename.c \
                        libmlk-util/mlk/util/openbsd/dirname.c \
                        libmlk-util/mlk/util/openbsd/getopt.c \
                        libmlk-util/mlk/util/openbsd/strlcat.c \
                        libmlk-util/mlk/util/openbsd/strlcpy.c
LIBMLK_UTIL_OBJS :=     $(LIBMLK_UTIL_SRCS:.c=.o)
LIBMLK_UTIL_DEPS :=     $(LIBMLK_UTIL_SRCS:.c=.d)

-include $(LIBMLK_UTIL_DEPS)

$(LIBMLK_UTIL): INCS :=
$(LIBMLK_UTIL): OBJS := $(LIBMLK_UTIL_OBJS)
$(LIBMLK_UTIL): $(LIBMLK_UTIL_OBJS)

all: $(LIBMLK_UTIL)

# }}}

# {{{ mlk-bcc

MLK_BCC :=              mlk-bcc/mlk-bcc

$(MLK_BCC): INCS :=

all: $(MLK_BCC)

# }}}

# {{{ mlk-tileset

MLK_TILESET :=          mlk-tileset/mlk-tileset

$(MLK_TILESET): INCS := $(JANSSON_INCS)
$(MLK_TILESET): LIBS := $(JANSSON_LIBS)

all: $(MLK_TILESET)

# }}}

# {{{ mlk-map

MLK_MAP :=              mlk-map/mlk-map

$(MLK_MAP): INCS := -Ilibmlk-util $(JANSSON_INCS)
$(MLK_MAP): LIBS := $(LIBMLK_UTIL) $(JANSSON_LIBS)
$(MLK_MAP): $(LIBMLK_UTIL)

all: $(MLK_MAP)

# }}}

# {{{ libmlk-sqlite

LIBMLK_SQLITE :=          libmlk-sqlite.a
LIBMLK_SQLITE_SRCS :=     extern/libsqlite/sqlite3.c
LIBMLK_SQLITE_OBJS :=     $(LIBMLK_SQLITE_SRCS:.c=.o)
LIBMLK_SQLITE_DEPS :=     $(LIBMLK_SQLITE_SRCS:.c=.d)

-include $(LIBMLK_SQLITE_DEPS)

$(LIBMLK_SQLITE_OBJS): private DEFS += -DSQLITE_THREADSAFE=0 \
                                       -DSQLITE_DEFAULT_MEMSTATUS=0 \
                                       -DSQLITE_OMIT_DECLTYPE \
                                       -DSQLITE_OMIT_DEPRECATED \
                                       -DSQLITE_OMIT_LOAD_EXTENSION \
                                       -DSQLITE_DEFAULT_FOREIGN_KEY=0

$(LIBMLK_SQLITE): private INCS :=
$(LIBMLK_SQLITE): private OBJS := $(LIBMLK_SQLITE_OBJS)
$(LIBMLK_SQLITE): $(LIBMLK_SQLITE_OBJS)

all: $(LIBMLK_SQLITE)

# }}}

# {{{ libmlk-core

LIBMLK_CORE :=          libmlk-core.a
LIBMLK_CORE_SRCS :=     libmlk-core/mlk/core/action-stack.c \
                        libmlk-core/mlk/core/action.c \
                        libmlk-core/mlk/core/alloc.c \
                        libmlk-core/mlk/core/animation.c \
                        libmlk-core/mlk/core/buf.c \
                        libmlk-core/mlk/core/clock.c \
                        libmlk-core/mlk/core/core.c \
                        libmlk-core/mlk/core/drawable-stack.c \
                        libmlk-core/mlk/core/drawable.c \
                        libmlk-core/mlk/core/err.c \
                        libmlk-core/mlk/core/error.c \
                        libmlk-core/mlk/core/event.c \
                        libmlk-core/mlk/core/font.c \
                        libmlk-core/mlk/core/game.c \
                        libmlk-core/mlk/core/image.c \
                        libmlk-core/mlk/core/maths.c \
                        libmlk-core/mlk/core/music.c \
                        libmlk-core/mlk/core/painter.c \
                        libmlk-core/mlk/core/panic.c \
                        libmlk-core/mlk/core/script.c \
                        libmlk-core/mlk/core/sound.c \
                        libmlk-core/mlk/core/sprite.c \
                        libmlk-core/mlk/core/state.c \
                        libmlk-core/mlk/core/sys.c \
                        libmlk-core/mlk/core/texture.c \
                        libmlk-core/mlk/core/trace.c \
                        libmlk-core/mlk/core/util.c \
                        libmlk-core/mlk/core/window.c
LIBMLK_CORE_OBJS :=     $(LIBMLK_CORE_SRCS:.c=.o)
LIBMLK_CORE_DEPS :=     $(LIBMLK_CORE_SRCS:.c=.d)

-include $(LIBMLK_CORE_DEPS)

$(LIBMLK_CORE): private OBJS := $(LIBMLK_CORE_OBJS)
$(LIBMLK_CORE): $(LIBMLK_CORE_OBJS)

all: $(LIBMLK_CORE)

# }}}

# {{{ libmlk-ui

LIBMLK_UI :=            libmlk-ui.a
LIBMLK_UI_SRCS :=       libmlk-ui/mlk/ui/align.c \
                        libmlk-ui/mlk/ui/button.c \
                        libmlk-ui/mlk/ui/checkbox.c \
                        libmlk-ui/mlk/ui/debug.c \
                        libmlk-ui/mlk/ui/frame.c \
                        libmlk-ui/mlk/ui/gridmenu.c \
                        libmlk-ui/mlk/ui/label.c \
                        libmlk-ui/mlk/ui/notify.c \
                        libmlk-ui/mlk/ui/theme.c \
                        libmlk-ui/mlk/ui/ui.c

LIBMLK_UI_OBJS :=       $(LIBMLK_UI_SRCS:.c=.o)
LIBMLK_UI_DEPS :=       $(LIBMLK_UI_SRCS:.c=.d)
LIBMLK_UI_DATA_SRCS :=  libmlk-ui/assets/fonts/opensans-light.ttf \
                        libmlk-ui/assets/fonts/opensans-medium.ttf \
                        libmlk-ui/assets/fonts/opensans-regular.ttf
LIBMLK_UI_DATA_OBJS :=  $(addsuffix .h,$(basename $(LIBMLK_UI_DATA_SRCS)))

$(LIBMLK_UI_DATA_OBJS): $(MLK_BCC)

$(LIBMLK_UI_OBJS): $(LIBMLK_UI_DATA_OBJS)

$(LIBMLK_UI): $(LIBMLK_UI_OBJS)
$(LIBMLK_UI): private OBJS := $(LIBMLK_UI_OBJS)

all: $(LIBMLK_UI)

# }}}

# {{{ libmlk-rpg

LIBMLK_RPG :=           libmlk-rpg.a
LIBMLK_RPG_SRCS :=      libmlk-rpg/mlk/rpg/battle-bar-default.c \
                        libmlk-rpg/mlk/rpg/battle-bar.c \
                        libmlk-rpg/mlk/rpg/battle-entity-state-attacking.c \
                        libmlk-rpg/mlk/rpg/battle-entity-state-blinking.c \
                        libmlk-rpg/mlk/rpg/battle-entity-state-moving.c \
                        libmlk-rpg/mlk/rpg/battle-entity-state-normal.c \
                        libmlk-rpg/mlk/rpg/battle-entity-state.c \
                        libmlk-rpg/mlk/rpg/battle-entity.c \
                        libmlk-rpg/mlk/rpg/battle-indicator.c \
                        libmlk-rpg/mlk/rpg/battle-message.c \
                        libmlk-rpg/mlk/rpg/battle-state-ai.c \
                        libmlk-rpg/mlk/rpg/battle-state-attacking.c \
                        libmlk-rpg/mlk/rpg/battle-state-check.c \
                        libmlk-rpg/mlk/rpg/battle-state-closing.c \
                        libmlk-rpg/mlk/rpg/battle-state-item.c \
                        libmlk-rpg/mlk/rpg/battle-state-lost.c \
                        libmlk-rpg/mlk/rpg/battle-state-menu.c \
                        libmlk-rpg/mlk/rpg/battle-state-opening.c \
                        libmlk-rpg/mlk/rpg/battle-state-rendering.c \
                        libmlk-rpg/mlk/rpg/battle-state-selection.c \
                        libmlk-rpg/mlk/rpg/battle-state-victory.c \
                        libmlk-rpg/mlk/rpg/battle-state.c \
                        libmlk-rpg/mlk/rpg/battle.c \
                        libmlk-rpg/mlk/rpg/character.c \
                        libmlk-rpg/mlk/rpg/equipment.c \
                        libmlk-rpg/mlk/rpg/inventory.c \
                        libmlk-rpg/mlk/rpg/item.c \
                        libmlk-rpg/mlk/rpg/map-file.c \
                        libmlk-rpg/mlk/rpg/map.c \
                        libmlk-rpg/mlk/rpg/message.c \
                        libmlk-rpg/mlk/rpg/property.c \
                        libmlk-rpg/mlk/rpg/quest.c \
                        libmlk-rpg/mlk/rpg/rpg.c \
                        libmlk-rpg/mlk/rpg/save.c \
                        libmlk-rpg/mlk/rpg/selection.c \
                        libmlk-rpg/mlk/rpg/spell.c \
                        libmlk-rpg/mlk/rpg/team.c \
                        libmlk-rpg/mlk/rpg/tileset-file.c \
                        libmlk-rpg/mlk/rpg/tileset.c \
                        libmlk-rpg/mlk/rpg/walksprite.c
LIBMLK_RPG_OBJS :=      $(LIBMLK_RPG_SRCS:.c=.o)
LIBMLK_RPG_DEPS :=      $(LIBMLK_RPG_SRCS:.c=.d)

LIBMLK_RPG_DATA_SRCS := libmlk-rpg/assets/sql/character-load.sql \
                        libmlk-rpg/assets/sql/character-save.sql \
                        libmlk-rpg/assets/sql/init.sql \
                        libmlk-rpg/assets/sql/property-load.sql \
                        libmlk-rpg/assets/sql/property-remove.sql \
                        libmlk-rpg/assets/sql/property-save.sql \
                        libmlk-rpg/assets/sql/quest-remove.sql \
                        libmlk-rpg/assets/sql/quest-save.sql \
                        libmlk-rpg/assets/sql/quest-step-load.sql \
                        libmlk-rpg/assets/sql/quest-step-save.sql
LIBMLK_RPG_DATA_OBJS := $(addsuffix .h,$(basename $(LIBMLK_RPG_DATA_SRCS)))

$(LIBMLK_RPG_DATA_OBJS): BCC_OPTS := -cs0
$(LIBMLK_RPG_DATA_OBJS): $(MLK_BCC)

$(LIBMLK_RPG_OBJS): $(LIBMLK_RPG_DATA_OBJS)

$(LIBMLK_RPG): $(LIBMLK_RPG_OBJS)
$(LIBMLK_RPG): private OBJS := $(LIBMLK_RPG_OBJS)

all: $(LIBMLK_RPG)

# }}}

# {{{ libmlk-example

LIBMLK_EXAMPLE :=               libmlk-example.a
LIBMLK_EXAMPLE_SRCS :=          examples/example/character-john.c \
                                examples/example/registry.c \
                                examples/example/spell-fire.c \
                                examples/example/trace_hud.c

LIBMLK_EXAMPLE_DATA_SRCS :=     examples/assets/images/battle-background.png \
                                examples/assets/images/black-cat.png \
                                examples/assets/images/haunted-wood.png \
                                examples/assets/images/sword.png \
                                examples/assets/music/vabsounds-romance.ogg \
                                examples/assets/sounds/fire.wav \
                                examples/assets/sprites/chest.png \
                                examples/assets/sprites/explosion.png \
                                examples/assets/sprites/john-sword.png \
                                examples/assets/sprites/john-walk.png \
                                examples/assets/sprites/numbers.png \
                                examples/assets/sprites/people.png \
                                examples/assets/sprites/ui-cursor.png
LIBMLK_EXAMPLE_DATA_OBJS :=     $(addsuffix .h,$(basename $(LIBMLK_EXAMPLE_DATA_SRCS)))

LIBMLK_EXAMPLE_OBJS :=          $(LIBMLK_EXAMPLE_SRCS:.c=.o)
LIBMLK_EXAMPLE_DEPS :=          $(LIBMLK_EXAMPLE_SRCS:.c=.d)

-include $(LIBMLK_EXAMPLE_DEPS)

$(LIBMLK_EXAMPLE_DATA_OBJS): $(MLK_BCC)

$(LIBMLK_EXAMPLE_OBJS): private INCS += -Iexamples
$(LIBMLK_EXAMPLE_OBJS): $(LIBMLK_EXAMPLE_DATA_OBJS)

$(LIBMLK_EXAMPLE): private OBJS := $(LIBMLK_EXAMPLE_OBJS)
$(LIBMLK_EXAMPLE): $(LIBMLK_EXAMPLE_OBJS)

# }}}

# {{{ examples

EXAMPLES :=             examples/example-action/example-action.c \
                        examples/example-animation/example-animation.c \
                        examples/example-audio/example-audio.c \
                        examples/example-cursor/example-cursor.c \
                        examples/example-debug/example-debug.c \
                        examples/example-drawable/example-drawable.c \
                        examples/example-font/example-font.c \
                        examples/example-gridmenu/example-gridmenu.c \
                        examples/example-label/example-label.c \
                        examples/example-message/example-message.c \
                        examples/example-notify/example-notify.c \
                        examples/example-sprite/example-sprite.c \
                        examples/example-ui/example-ui.c
EXAMPLES_EXE :=         $(EXAMPLES:.c=)
EXAMPLES_OBJS :=        $(EXAMPLES_EXE)

$(EXAMPLES_EXE): private LIBS += $(LIBMLK) $(LIBMLK_EXAMPLE)
$(EXAMPLES_EXE): private INCS += -Iexamples
$(EXAMPLES_EXE): $(LIBMLK_RPG) $(LIBMLK_UI) $(LIBMLK_CORE) $(LIBMLK_UTIL) $(LIBMLK_SQLITE) $(LIBMLK_EXAMPLE)

examples: $(EXAMPLES_EXE)

# }}}

# {{{ tests

TESTS :=        tests/test-action-script.c \
                tests/test-action.c \
                tests/test-alloc.c \
                tests/test-character.c \
                tests/test-color.c \
                tests/test-drawable.c \
                tests/test-error.c \
                tests/test-map.c \
                tests/test-save-quest.c \
                tests/test-save.c \
                tests/test-state.c \
                tests/test-tileset.c \
                tests/test-util.c

TESTS_EXE :=    $(TESTS:.c=)

$(TESTS_EXE): private LIBS += $(LIBMLK)
$(TESTS_EXE): private DEFS += -DDIRECTORY=\"$(CURDIR)/tests/assets\"
$(TESTS_EXE): $(LIBMLK_RPG) $(LIBMLK_UI) $(LIBMLK_CORE) $(LIBMLK_UTIL) $(LIBMLK_SQLITE)

tests: $(TESTS_EXE)
	for t in $(TESTS_EXE); do ./$$t; done

# }}}

# {{{ manual pages

MAN3 :=         man/mlk-action.3 \
                man/mlk-alloc.3 \
                man/mlk-err.3 \
                man/mlk_err_string.3
                man/mlk_action_draw.3 \
                man/mlk_action_end.3 \
                man/mlk_action_finish.3 \
                man/mlk_action_handle.3 \
                man/mlk_action_update.3

# }}}

doc:
	cd doc && mkdocs build

doc-serve:
	cd doc && mkdocs serve

install:
	mkdir -p $(DESTDIR)$(LIBDIR)
	mkdir -p $(DESTDIR)$(INCDIR)/mlk/core
	mkdir -p $(DESTDIR)$(INCDIR)/mlk/ui
	mkdir -p $(DESTDIR)$(INCDIR)/mlk/rpg
	cp $(LIBMLK_SQLITE) $(LIBMLK_CORE) $(LIBMLK_UI) $(LIBMLK_RPG) $(DESTDIR)$(LIBDIR)
	cp -R libmlk-core/mlk/core/*.h $(DESTDIR)$(INCDIR)/mlk/core
	cp -R libmlk-ui/mlk/ui/*.h $(DESTDIR)$(INCDIR)/mlk/ui
	cp -R libmlk-rpg/mlk/rpg/*.h $(DESTDIR)$(INCDIR)/mlk/rpg
	mkdir -p $(DESTDIR)$(MANDIR)/man3
	cp $(MAN3) $(DESTDIR)$(MANDIR)/man3

clean:
	rm -f $(MLK_BCC) $(MLK_MAP) $(MLK_TILESET)
	rm -f $(LIBMLK_SQLITE) $(LIBMLK_SQLITE_DEPS) $(LIBMLK_SQLITE_OBJS)
	rm -f $(LIBMLK_UTIL) $(LIBMLK_UTIL_DEPS) $(LIBMLK_UTIL_OBJS)
	rm -f $(LIBMLK_CORE) $(LIBMLK_CORE_DEPS) $(LIBMLK_CORE_OBJS)
	rm -f $(LIBMLK_UI) $(LIBMLK_UI_DEPS) $(LIBMLK_UI_OBJS) $(LIBMLK_UI_DATA_OBJS)
	rm -f $(LIBMLK_RPG) $(LIBMLK_RPG_DEPS) $(LIBMLK_RPG_OBJS) $(LIBMLK_RPG_DATA_OBJS)
	rm -f $(LIBMLK_EXAMPLE) $(LIBMLK_EXAMPLE_DEPS) $(LIBMLK_EXAMPLE_OBJS) $(LIBMLK_EXAMPLE_DATA_OBJS)
	rm -f $(TESTS_EXE) $(EXAMPLES_EXE)

.PHONY: all clean doc doc-serve examples install tests
