/*
 * molko-map.c -- convert tiled .tmx into custom files
 *
 * Copyright (c) 2020 David Demelier <markand@malikania.fr>
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
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <expat.h>

enum state {
	STATE_ROOT,             /* no tag read yet */
	STATE_MAP,              /* top <map> */
	STATE_MAP_PROPERTIES,   /* <map><properties> */
	STATE_TILESET,          /* <tileset> */
	STATE_TILESET_IMAGE,    /* <tileset><image> */
	STATE_LAYER,            /* <layer> */
	STATE_LAYER_DATA,       /* <layer><data> */
};

enum layer {
	LAYER_NONE,
	LAYER_BACKGROUND,
	LAYER_FOREGROUND
};

struct context {
	enum state state;
	enum layer layer;
};

static const int layer_indices[] = {
	[LAYER_NONE] = -1,
	[LAYER_BACKGROUND] = 0,
	[LAYER_FOREGROUND] = 1
};

static void on_data(struct context *, const char *, const char **);
static void on_image(struct context *, const char *, const char **);
static void on_layer(struct context *, const char *, const char **);
static void on_map(struct context *, const char *, const char **);
static void on_properties(struct context *, const char *, const char **);
static void on_property(struct context *, const char *, const char **);
static void on_tile(struct context *, const char *, const char **);
static void on_tileset(struct context *, const char *, const char **);
static void die(const char *, ...);

static const struct {
	const char *name;
	void (*begin)(struct context *, const char *, const char **);
	void (*end)(struct context *, const char *);
} handlers[] = {
	{ "data",       on_data,        NULL    },
	{ "image",      on_image,       NULL    },
	{ "layer",      on_layer,       NULL    },
	{ "map",        on_map,         NULL    },
	{ "properties", on_properties,  NULL    },
	{ "property",   on_property,    NULL    },
	{ "tile",       on_tile,        NULL    },
	{ "tileset",    on_tileset,     NULL    },
	{ NULL,         NULL,           NULL    },
};

static bool
is_known_map_property(const char *name)
{
	static const char *known[] = {
		"title",
		NULL
	};

	for (size_t i = 0; known[i]; ++i)
		if (strcmp(known[i], name) == 0)
			return true;

	return false;
}

static void
write_map_property(const char **attrs)
{
	/*
	 * This is how <map><properties> are defined:
	 *
	 *             0         1          2          3
	 * <property name="PROPERTY NAME" value="PROPERTY VALUE">
	 */
	for (size_t i = 0; attrs[i]; i += 4) {
		if (is_known_map_property(attrs[i + 1]))
			printf("%s|%s\n", attrs[i + 1], attrs[i + 3]);
		else
			die("unsupported map property: %s\n", attrs[i + 1]);
	}
}

static const char *
find_attr(const char **attrs, const char *key)
{
	for (size_t i = 0; attrs[i]; i += 2)
		if (strcmp(attrs[i], key) == 0)
			return attrs[i + 1];

	return NULL;
}

static void
on_map(struct context *ctx, const char *name, const char **attrs)
{
	(void)ctx;
	(void)name;
	(void)attrs;

	ctx->state = STATE_MAP;
}

static void
on_properties(struct context *ctx, const char *name, const char **attrs)
{
	(void)name;
	(void)attrs;

	/*
	 * <properties> exist in different parent tags
	 */
	switch (ctx->state) {
	case STATE_MAP:
		ctx->state = STATE_MAP_PROPERTIES;
		break;
	default:
		fprintf(stderr, "warning: unsupported <properties>\n");
		break;
	}
}

static void
on_property(struct context *ctx, const char *name, const char **attrs)
{
	(void)name;
	(void)attrs;

	switch (ctx->state) {
	case STATE_MAP_PROPERTIES:
		write_map_property(attrs);
		break;
	default:
		break;
	}
}

static void
on_layer(struct context *ctx, const char *name, const char **attrs)
{
	(void)name;

	/*
	 * The layer is defined like this:
	 *
	 * Values of attrs:
	 *          0       1       IGNORED
	 * <layer name="background" width="10" height="10">
	 */
	const char *layername = find_attr(attrs, "name");

	if (!layername)
		die("layer is missing 'name' attribute\n");

	if (strcmp(layername, "background") == 0)
		ctx->layer = LAYER_BACKGROUND;
	else
		die("invalid '%s' value for 'name' property\n", layername);

	ctx->state = STATE_LAYER;
}

static void
on_data(struct context *ctx, const char *name, const char **attrs)
{
	(void)name;
	(void)attrs;

	if (ctx->state != STATE_LAYER)
		die("reading <data> outside a <layer> node\n");

	ctx->state = STATE_LAYER_DATA;
}

static void
on_tile(struct context *ctx, const char *name, const char **attrs)
{
	(void)name;

	/* TODO: add more layer */
	if (ctx->state != STATE_LAYER_DATA)
		die("reading <tile> outside a <data> node\n");

	/*
	 * The tile is defined as following:
	 *
	 * Values of attrs:
	 *        0   1
	 * <tile gid="1" />
	 */
	const char *gid = find_attr(attrs, "gid");

	if (!gid)
		die("missing 'gid' attribute in <tile> node\n");

	printf("tile|%d|%s\n", layer_indices[ctx->layer], gid);
}

static void
on_tileset(struct context *ctx, const char *name, const char **attrs)
{
	(void)ctx;
	(void)name;
	(void)attrs;

	ctx->state = STATE_TILESET;
}

static void
on_image(struct context *ctx, const char *name, const char **attrs)
{
	(void)name;

	if (ctx->state != STATE_TILESET)
		die("reading <image> outside a <tileset> node\n");

	/**
	 * The tile is defined as following:
	 *
	 * Values of attrs:
	 *        0   1
	 * <image source="name.png" ... />
	 */
	const char *src = find_attr(attrs, "source");

	if (!src)
		die("missing 'source' attribute in <image> node\n");

	printf("tileset|%s\n", src);
}

static void
on_start_element(struct context *ctx, const char *name, const char **attrs)
{
	assert(ctx);
	assert(name);
	assert(attrs);

	size_t i;

	for (i = 0; handlers[i].name; ++i) {
		if (strcmp(name, handlers[i].name) == 0) {
			handlers[i].begin(ctx, name, attrs);
			break;
		}
	}

	if (!handlers[i].name)
		fprintf(stderr, "unsupported <%s> tag\n", name);
}

static void
on_end_element(void *data, const char *name)
{
	(void)data;
	(void)name;
#if 0
	size_t i;

	for (i = 0; handlers[i].name; ++i) {
		if (strcmp(name, handlers[i].name) == 0) {
			handlers[i].end(ctx, name, attrs);
			break;
		}
	}

	if (!handlers[i].name)
		fprintf(stderr, "unsupported <%s> tag\n", name);
#endif
}

static void
run(XML_Parser parser)
{
	char buffer[BUFSIZ];
	size_t nbread;
	struct context ctx = {0};

	XML_SetUserData(parser, &ctx);

	while ((nbread = fread(buffer, 1, sizeof (buffer), stdin)) > 0) {
		bool done = nbread < sizeof (buffer);

		if (XML_Parse(parser, buffer, nbread, done) == XML_STATUS_ERROR) {
			fprintf(stderr, "%lu: %s\n",
			    XML_GetCurrentLineNumber(parser),
			    XML_ErrorString(XML_GetErrorCode(parser)));
			exit(1);
		}
	}
}

static void
die(const char *fmt, ...)
{
	assert(fmt);

	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	exit(1);
}

int
main(void)
{
	XML_Parser parser;

	parser = XML_ParserCreate(NULL);
	XML_SetElementHandler(parser,
		(XML_StartElementHandler)on_start_element, on_end_element);

	run(parser);

	XML_ParserFree(parser);
}
