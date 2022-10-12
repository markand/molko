-include config.mk

# Toolchain options.
CC ?=                   clang
CFLAGS ?=               -g -O0

# Installation directory.
PREFIX ?=               /usr/local
LIBDIR ?=               $(PREFIX)/lib
INCDIR ?=               $(PREFIX)/include

# User options.
WITH_ZIP ?=             yes
WITH_ZSTD ?=            yes
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

ifeq ($(WITH_ZSTD),yes)
ZSTD_INCS ?=            $(shell pkg-config --cflags libzstd)
ZSTD_LIBS ?=            $(shell pkg-config --libs libzstd)
endif

ifeq ($(WITH_ZIP),yes)
LIBZIP_INCS ?=          $(shell pkg-config --cflags libzip)
LIBZIP_LIBS ?=          $(shell pkg-config --libs libzip)
endif

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
                        -Isrc/libmlk-port \
                        -Isrc/libmlk-core \
                        -Isrc/libmlk-ui \
                        -Isrc/libmlk-rpg \
                        -I. \
                        $(SDL2_INCS) \
                        $(SDL2_IMAGE_INCS) \
                        $(SDL2_TTF_INCS) \
                        $(OPENAL_INCS) \
                        $(SNDFILE_INCS)

ifeq ($(WITH_ZSTD),yes)
INCS +=                 $(ZSTD_INCS)
endif

ifeq ($(WITH_ZIP),yes)
INCS +=                 $(ZIP_INCS)
endif

OPTS :=                 -Wall -Wextra -pipe

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
                        $(LIBMLK_PORT) \
                        $(LIBMLK_SQLITE) \
                        $(MATH_LIBS) \
                        $(SDL2_LIBS) \
                        $(SDL2_IMAGE_LIBS) \
                        $(SDL2_TTF_LIBS) \
                        $(OPENAL_LIBS) \
                        $(SNDFILE_LIBS)

ifeq ($(WITH_ZSTD),yes)
LIBMLK +=               $(ZSTD_LIBS)
endif

ifeq ($(WITH_ZIP),yes)
LIBMLK +=               $(LIBZIP_LIBS)
endif

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

# {{{ config.h

config.h:
	rm -f config.h
	touch config.h
ifeq ($(WITH_ZSTD),yes)
	echo "#define MLK_WITH_ZSTD" >> config.h
endif
ifeq ($(WITH_ZIP),yes)
	echo "#define MLK_WITH_ZIP" >> config.h
endif

# }}}

# {{{ libmlk-port

LIBMLK_PORT :=          libmlk-port.a
LIBMLK_PORT_SRCS :=     src/libmlk-port/port/basename.c \
                        src/libmlk-port/port/dirname.c \
                        src/libmlk-port/port/fmemopen.c \
                        src/libmlk-port/port/getopt.c \
                        src/libmlk-port/port/strlcat.c \
                        src/libmlk-port/port/strlcpy.c
LIBMLK_PORT_OBJS :=     $(LIBMLK_PORT_SRCS:.c=.o)
LIBMLK_PORT_DEPS :=     $(LIBMLK_PORT_SRCS:.c=.d)

-include $(LIBMLK_PORT_DEPS)

$(LIBMLK_PORT): INCS :=
$(LIBMLK_PORT): OBJS := $(LIBMLK_PORT_OBJS)
$(LIBMLK_PORT): $(LIBMLK_PORT_OBJS)

all: $(LIBMLK_PORT)

# }}}

# {{{ mlk-bcc

MLK_BCC :=              src/tools/bcc/mlk-bcc

$(MLK_BCC): INCS :=

all: $(MLK_BCC)

# }}}

# {{{ mlk-tileset

MLK_TILESET :=          src/tools/tileset/mlk-tileset

$(MLK_TILESET): INCS := $(JANSSON_INCS)
$(MLK_TILESET): LIBS := $(JANSSON_LIBS)

all: $(MLK_TILESET)

# }}}

# {{{ mlk-map

MLK_MAP :=              src/tools/map/mlk-map

$(MLK_MAP): INCS := -Isrc/libmlk-port $(JANSSON_INCS)
$(MLK_MAP): LIBS := $(LIBMLK_PORT) $(JANSSON_LIBS)
$(MLK_MAP): $(LIBMLK_PORT)

all: $(MLK_MAP)

# }}}

# {{{ libmlk-sqlite

LIBMLK_SQLITE :=          libmlk-sqlite.a
LIBMLK_SQLITE_SRCS :=     extern/libsqlite/sqlite3.c
LIBMLK_SQLITE_OBJS :=     $(LIBMLK_SQLITE_SRCS:.c=.o)
LIBMLK_SQLITE_DEPS :=     $(LIBMLK_SQLITE_SRCS:.c=.d)

-include $(LIBMLK_SQLITE_DEPS)

$(LIBMLK_SQLITE): private INCS :=
$(LIBMLK_SQLITE): private OBJS := $(LIBMLK_SQLITE_OBJS)
$(LIBMLK_SQLITE): $(LIBMLK_SQLITE_OBJS)

all: $(LIBMLK_SQLITE)

# }}}

# {{{ libmlk-core

LIBMLK_CORE :=          libmlk-core.a
LIBMLK_CORE_SRCS :=     src/libmlk-core/core/action-stack.c \
                        src/libmlk-core/core/action.c \
                        src/libmlk-core/core/alloc.c \
                        src/libmlk-core/core/animation.c \
                        src/libmlk-core/core/buf.c \
                        src/libmlk-core/core/clock.c \
                        src/libmlk-core/core/core.c \
                        src/libmlk-core/core/drawable-stack.c \
                        src/libmlk-core/core/drawable.c \
                        src/libmlk-core/core/err.c \
                        src/libmlk-core/core/error.c \
                        src/libmlk-core/core/event.c \
                        src/libmlk-core/core/font.c \
                        src/libmlk-core/core/game.c \
                        src/libmlk-core/core/image.c \
                        src/libmlk-core/core/maths.c \
                        src/libmlk-core/core/music.c \
                        src/libmlk-core/core/painter.c \
                        src/libmlk-core/core/panic.c \
                        src/libmlk-core/core/script.c \
                        src/libmlk-core/core/sound.c \
                        src/libmlk-core/core/sprite.c \
                        src/libmlk-core/core/state.c \
                        src/libmlk-core/core/sys.c \
                        src/libmlk-core/core/texture.c \
                        src/libmlk-core/core/trace.c \
                        src/libmlk-core/core/util.c \
                        src/libmlk-core/core/vfs-directory.c \
                        src/libmlk-core/core/vfs-zip.c \
                        src/libmlk-core/core/vfs.c \
                        src/libmlk-core/core/window.c \
                        src/libmlk-core/core/zfile.c
LIBMLK_CORE_OBJS :=     $(LIBMLK_CORE_SRCS:.c=.o)
LIBMLK_CORE_DEPS :=     $(LIBMLK_CORE_SRCS:.c=.d)

-include $(LIBMLK_CORE_DEPS)

$(LIBMLK_CORE_OBJS): config.h
$(LIBMLK_CORE): private OBJS := $(LIBMLK_CORE_OBJS)
$(LIBMLK_CORE): $(LIBMLK_CORE_OBJS)

all: $(LIBMLK_CORE)

# }}}

# {{{ libmlk-ui

LIBMLK_UI :=            libmlk-ui.a
LIBMLK_UI_SRCS :=       src/libmlk-ui/ui/align.c \
                        src/libmlk-ui/ui/button.c \
                        src/libmlk-ui/ui/checkbox.c \
                        src/libmlk-ui/ui/debug.c \
                        src/libmlk-ui/ui/frame.c \
                        src/libmlk-ui/ui/gridmenu.c \
                        src/libmlk-ui/ui/label.c \
                        src/libmlk-ui/ui/notify.c \
                        src/libmlk-ui/ui/theme.c \
                        src/libmlk-ui/ui/ui.c

LIBMLK_UI_OBJS :=       $(LIBMLK_UI_SRCS:.c=.o)
LIBMLK_UI_DEPS :=       $(LIBMLK_UI_SRCS:.c=.d)
LIBMLK_UI_DATA_SRCS :=  src/libmlk-ui/assets/fonts/opensans-light.ttf \
                        src/libmlk-ui/assets/fonts/opensans-medium.ttf \
                        src/libmlk-ui/assets/fonts/opensans-regular.ttf
LIBMLK_UI_DATA_OBJS :=  $(addsuffix .h,$(basename $(LIBMLK_UI_DATA_SRCS)))

$(LIBMLK_UI_DATA_OBJS): $(MLK_BCC)

$(LIBMLK_UI_OBJS): $(LIBMLK_UI_DATA_OBJS)

$(LIBMLK_UI): $(LIBMLK_UI_OBJS)
$(LIBMLK_UI): private OBJS := $(LIBMLK_UI_OBJS)

all: $(LIBMLK_UI)

# }}}

# {{{ libmlk-rpg

LIBMLK_RPG :=           libmlk-rpg.a
LIBMLK_RPG_SRCS :=      src/libmlk-rpg/rpg/battle-bar-default.c \
                        src/libmlk-rpg/rpg/battle-bar.c \
                        src/libmlk-rpg/rpg/battle-entity-state-attacking.c \
                        src/libmlk-rpg/rpg/battle-entity-state-blinking.c \
                        src/libmlk-rpg/rpg/battle-entity-state-moving.c \
                        src/libmlk-rpg/rpg/battle-entity-state-normal.c \
                        src/libmlk-rpg/rpg/battle-entity-state.c \
                        src/libmlk-rpg/rpg/battle-entity.c \
                        src/libmlk-rpg/rpg/battle-indicator.c \
                        src/libmlk-rpg/rpg/battle-message.c \
                        src/libmlk-rpg/rpg/battle-state-ai.c \
                        src/libmlk-rpg/rpg/battle-state-attacking.c \
                        src/libmlk-rpg/rpg/battle-state-check.c \
                        src/libmlk-rpg/rpg/battle-state-closing.c \
                        src/libmlk-rpg/rpg/battle-state-item.c \
                        src/libmlk-rpg/rpg/battle-state-lost.c \
                        src/libmlk-rpg/rpg/battle-state-menu.c \
                        src/libmlk-rpg/rpg/battle-state-opening.c \
                        src/libmlk-rpg/rpg/battle-state-rendering.c \
                        src/libmlk-rpg/rpg/battle-state-selection.c \
                        src/libmlk-rpg/rpg/battle-state-victory.c \
                        src/libmlk-rpg/rpg/battle-state.c \
                        src/libmlk-rpg/rpg/battle.c \
                        src/libmlk-rpg/rpg/character.c \
                        src/libmlk-rpg/rpg/equipment.c \
                        src/libmlk-rpg/rpg/inventory.c \
                        src/libmlk-rpg/rpg/item.c \
                        src/libmlk-rpg/rpg/map-file.c \
                        src/libmlk-rpg/rpg/map.c \
                        src/libmlk-rpg/rpg/message.c \
                        src/libmlk-rpg/rpg/property.c \
                        src/libmlk-rpg/rpg/quest.c \
                        src/libmlk-rpg/rpg/rpg.c \
                        src/libmlk-rpg/rpg/save.c \
                        src/libmlk-rpg/rpg/selection.c \
                        src/libmlk-rpg/rpg/spell.c \
                        src/libmlk-rpg/rpg/team.c \
                        src/libmlk-rpg/rpg/tileset-file.c \
                        src/libmlk-rpg/rpg/tileset.c \
                        src/libmlk-rpg/rpg/walksprite.c
LIBMLK_RPG_OBJS :=      $(LIBMLK_RPG_SRCS:.c=.o)
LIBMLK_RPG_DEPS :=      $(LIBMLK_RPG_SRCS:.c=.d)

LIBMLK_RPG_DATA_SRCS := src/libmlk-rpg/assets/sql/character-load.sql \
                        src/libmlk-rpg/assets/sql/character-save.sql \
                        src/libmlk-rpg/assets/sql/init.sql \
                        src/libmlk-rpg/assets/sql/property-load.sql \
                        src/libmlk-rpg/assets/sql/property-remove.sql \
                        src/libmlk-rpg/assets/sql/property-save.sql \
                        src/libmlk-rpg/assets/sql/quest-remove.sql \
                        src/libmlk-rpg/assets/sql/quest-save.sql \
                        src/libmlk-rpg/assets/sql/quest-step-load.sql \
                        src/libmlk-rpg/assets/sql/quest-step-save.sql
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
$(EXAMPLES_EXE): $(LIBMLK_RPG) $(LIBMLK_UI) $(LIBMLK_CORE) $(LIBMLK_PORT) $(LIBMLK_SQLITE) $(LIBMLK_EXAMPLE)

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
                tests/test-util.c \
                tests/test-vfs-directory.c \

ifeq ($(WITH_ZIP),yes)
TESTS +=        tests/test-vfs-zip.c
endif

TESTS_EXE :=    $(TESTS:.c=)

$(TESTS_EXE): private LIBS += $(LIBMLK)
$(TESTS_EXE): private DEFS += -DDIRECTORY=\"$(CURDIR)/tests/assets\"
$(TESTS_EXE): $(LIBMLK_RPG) $(LIBMLK_UI) $(LIBMLK_CORE) $(LIBMLK_PORT) $(LIBMLK_SQLITE)

tests: $(TESTS_EXE)
	for t in $(TESTS_EXE); do ./$$t; done

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
	cp -R src/libmlk-core/core/*.h $(DESTDIR)$(INCDIR)/mlk/core
	cp -R src/libmlk-ui/ui/*.h $(DESTDIR)$(INCDIR)/mlk/ui
	cp -R src/libmlk-rpg/rpg/*.h $(DESTDIR)$(INCDIR)/mlk/rpg

clean:
	rm -f config.h
	rm -f $(MLK_BCC) $(MLK_MAP) $(MLK_TILESET)
	rm -f $(LIBMLK_SQLITE) $(LIBMLK_SQLITE_DEPS) $(LIBMLK_SQLITE_OBJS)
	rm -f $(LIBMLK_PORT) $(LIBMLK_PORT_DEPS) $(LIBMLK_PORT_OBJS)
	rm -f $(LIBMLK_CORE) $(LIBMLK_CORE_DEPS) $(LIBMLK_CORE_OBJS)
	rm -f $(LIBMLK_UI) $(LIBMLK_UI_DEPS) $(LIBMLK_UI_OBJS) $(LIBMLK_UI_DATA_OBJS)
	rm -f $(LIBMLK_RPG) $(LIBMLK_RPG_DEPS) $(LIBMLK_RPG_OBJS) $(LIBMLK_RPG_DATA_OBJS)
	rm -f $(LIBMLK_EXAMPLE) $(LIBMLK_EXAMPLE_DEPS) $(LIBMLK_EXAMPLE_OBJS) $(LIBMLK_EXAMPLE_DATA_OBJS)
	rm -f $(TESTS_EXE) $(EXAMPLES_EXE)

.PHONY: all clean doc doc-serve examples install tests
