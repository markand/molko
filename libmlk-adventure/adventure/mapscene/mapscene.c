/*
 * mapscene.c -- map scene loader with actions
 *
 * Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <core/alloc.h>
#include <core/panic.h>
#include <core/util.h>

#include <rpg/map.h>

#include <adventure/action/teleport.h>
#include <adventure/action/spawner.h>

#include <adventure/trace_hud.h>

#include "mapscene.h"

#define SEARCH(key, array, size, cmp) \
	bsearch(key, array, UTIL_SIZE(array), size, (cmp_fn)cmp)

typedef int (*cmp_fn)(const void *, const void *);

struct action_loader {
	const char *name;
	void (*load)(struct map *, int, int, int, int, const char *);
};

struct mapscene_loader {
	const char *name;
	void (*load)(struct map *);
};

static void
finish(struct action *a)
{
	free(a->data);
}

void
load_teleport(struct map *m, int x, int y, int w, int h, const char *line)
{
	struct teleport *tp;

	tp = alloc_new0(sizeof (*tp));
	tp->x = x;
	tp->y = y;
	tp->w = w;
	tp->h = h;
	tp->map = m;

	if (sscanf(line, "|%255[^|]|%d|%d", tp->destination, &tp->origin_x, &tp->origin_y) < 1)
		panic();

	teleport_action(tp)->finish = finish;
	action_stack_add(&m->astack_par, &tp->action);
}

void
load_spawner(struct map *m, int x, int y, int w, int h, const char *line)
{
	(void)x;
	(void)y;
	(void)w;
	(void)h;

	struct spawner *s;

	s = alloc_new0(sizeof (*s));
	s->map = m;

	sscanf(line, "|%u|%u", &s->low, &s->high);

	spawner_action(s)->finish = finish;
	action_stack_add(&m->astack_par, &s->action);
}

/* Default objects supported on all maps. Must be sorted. */
static const struct action_loader loaders[] = {
	{ "spawner",    load_spawner    },
	{ "teleport",   load_teleport   }
};

#if 0

/* Per map actions, name refer to map names. Must be sorted. */
static const struct action_loader maploaders[] = {
};

/* Per map loaders, to add extra data. Must be sorted. */
static const struct mapscene_loader mapscenes[] = {
};

#endif

static int
cmp_name(const char *key, const struct action_loader *ld)
{
	return strncmp(key, ld->name, strlen(ld->name));
}

#if 0

static int
cmp_title(const char *key, const struct action_loader *ld)
{
	return strcmp(key, ld->name);
}

#endif

void
mapscene_load_action(struct map *m, int x, int y, int w, int h, const char *exec)
{
	assert(m);
	assert(exec);

	struct action_loader *ld;

	/* 1. Load predefined loaders. */
	if ((ld = SEARCH(exec, loaders, sizeof (*ld), cmp_name)))
		ld->load(m, x, y, w, h, exec + strcspn(exec, "|"));

#if 0
	/* 2. Load per map objects. */
	if ((ld = SEARCH(m->title, maploaders, sizeof (*ld), cmp_title)))
		ld->load(m, x, y, w, h, exec);
#endif
}

void
mapscene_load(struct map *m)
{
	assert(m);

#if 0
	struct mapscene_loader *ld;

	/* Same layout, can use cmp_title as well. */
	if ((ld = SEARCH(m->title, mapscenes, sizeof (*ld), cmp_title)))
		ld->load(m);
#endif

	/* Add the trace hud. */
	action_stack_add(&m->astack_par, trace_hud_action());
}
