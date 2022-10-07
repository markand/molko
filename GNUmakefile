-include config.mk

# Toolchain options.
CC ?=                   clang
CFLAGS ?=               -g -O0
BCC ?=                  bcc
BCC_OPTS ?=             -cs
MD ?=                   -MD

# User options.
WITH_ZIP :=             yes
WITH_ZSTD :=            yes
WITH_DEBUG :=           no

OS :=                   $(shell uname -s)

SDL2_INCS ?=            $(shell pkg-config --cflags sdl2)
SDL2_LIBS ?=            $(shell pkg-config --libs sdl2)

SDL2_IMAGE_INCS ?=      $(shell pkg-config --cflags SDL2_image)
SDL2_IMAGE_LIBS ?=      $(shell pkg-config --libs SDL2_image)

SDL2_TTF_INCS ?=        $(shell pkg-config --cflags SDL2_ttf)
SDL2_TTF_LIBS ?=        $(shell pkg-config --libs SDL2_ttf)

SNDFILE_INCS ?=         $(shell pkg-config --cflags sndfile)
SNDFILE_LIBS ?=         $(shell pkg-config --libs sndfile)

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

.DEFAULT_GOAL :=        all

.SUFFIXES:
.SUFFIXES: .c .h .o .sql .ttf

.c.o:
	$(CC) $(OPTS) $(INCS) $(DEFS) $(CFLAGS) $(MD) -c $< -o $@

.c:
	$(CC) $(OPTS) $(INCS) $(DEFS) $(CFLAGS) $< -o $@ $(LIBS) $(LDFLAGS)

.ttf.h .sql.h:
	$(BCC) $(BCC_OPTS) $< assets_$(basename $(<F)) > $@

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

# {{{ libmlk-sqlite

LIBMLK_SQLITE :=          libmlk-sqlite.a
LIBMLK_SQLITE_SRCS :=     extern/libsqlite/sqlite3.c
LIBMLK_SQLITE_OBJS :=     $(LIBMLK_SQLITE_SRCS:.c=.o)
LIBMLK_SQLITE_DEPS :=     $(LIBMLK_SQLITE_SRCS:.c=.d)

-include $(LIBMLK_SQLITE_DEPS)

$(LIBMLK_SQLITE): INCS :=
$(LIBMLK_SQLITE): OBJS := $(LIBMLK_SQLITE_OBJS)
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
$(LIBMLK_CORE): OBJS := $(LIBMLK_CORE_OBJS)
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

$(LIBMLK_UI_OBJS): $(LIBMLK_UI_DATA_OBJS)
$(LIBMLK_UI): $(LIBMLK_UI_OBJS)
$(LIBMLK_UI): OBJS := $(LIBMLK_UI_OBJS)

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
$(LIBMLK_RPG_OBJS): $(LIBMLK_RPG_DATA_OBJS)
$(LIBMLK_RPG): $(LIBMLK_RPG_OBJS)
$(LIBMLK_RPG): OBJS := $(LIBMLK_RPG_OBJS)

all: $(LIBMLK_RPG)

# }}}

# Meta variable that contains all libraries, used for executables and tests.
LIBMLK :=       $(LIBMLK_RPG) \
                $(LIBMLK_UI) \
                $(LIBMLK_CORE) \
                $(LIBMLK_PORT) \
                $(LIBMLK_SQLITE) \
                $(SDL2_LIBS) \
                $(SDL2_IMAGE_LIBS) \
                $(SDL2_TTF_LIBS) \
                $(OPENAL_LIBS) \
                $(SNDFILE_LIBS)

ifeq ($(WITH_ZSTD),yes)
LIBMLK +=       $(ZSTD_LIBS)
endif

ifeq ($(WITH_ZIP),yes)
LIBMLK +=       $(LIBZIP_LIBS)
endif

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

clean:
	rm -f config.h
	rm -f $(LIBMLK_SQLITE) $(LIBMLK_SQLITE_DEPS) $(LIBMLK_SQLITE_OBJS)
	rm -f $(LIBMLK_PORT) $(LIBMLK_PORT_DEPS) $(LIBMLK_PORT_OBJS)
	rm -f $(LIBMLK_CORE) $(LIBMLK_CORE_DEPS) $(LIBMLK_CORE_OBJS)
	rm -f $(LIBMLK_UI) $(LIBMLK_UI_DEPS) $(LIBMLK_UI_OBJS) $(LIBMLK_UI_DATA_OBJS)
	rm -f $(LIBMLK_RPG) $(LIBMLK_RPG_DEPS) $(LIBMLK_RPG_OBJS) $(LIBMLK_RPG_DATA_OBJS)
	rm -f $(TESTS_EXE)

.PHONY: all clean tests
