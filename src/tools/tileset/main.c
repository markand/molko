/*
 * main.c -- convert tiled tilesets JSON files into custom files
 *
 * Copyright (c) 2020-2022 David Demelier <markand@malikania.fr>
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
#include <stdio.h>
#include <string.h>

#include <jansson.h>

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

static void
write_dimensions(const json_t *document)
{
	const json_t *tilewidth = json_object_get(document, "tilewidth");
	const json_t *tileheight = json_object_get(document, "tileheight");

	if (!json_is_integer(tilewidth))
		die("invalid 'tilewidth' property\n");
	if (!json_is_integer(tileheight))
		die("invalid 'tileheight' property\n");

	printf("tilewidth|%u\n", (unsigned int)json_integer_value(tilewidth));
	printf("tileheight|%u\n", (unsigned int)json_integer_value(tileheight));
}

static void
write_image(const json_t *document)
{
	const json_t *image = json_object_get(document, "image");

	if (!json_is_string(image))
		die("invalid 'image' property\n");

	printf("image|%s\n", json_string_value(image));
}

static const json_t *
find_property_value(const json_t *array, const char *prop)
{
	const json_t *obj;
	size_t i;

	json_array_foreach(array, i, obj) {
		const json_t *name = json_object_get(obj, "name");

		if (!name || !json_is_string(name))
			die("invalid property object\n");

		if (strcmp(json_string_value(name), prop) == 0)
			return json_object_get(obj, "value");
	}

	return NULL;
}

static void
write_animation(const json_t *tile)
{
	const json_t *id = json_object_get(tile, "id");
	const json_t *properties = json_object_get(tile, "properties");
	const json_t *file = find_property_value(properties, "animation-file");
	const json_t *delay = find_property_value(properties, "animation-delay");

	/* Animations are completely optional. */
	if (!json_is_array(properties))
		return;

	if (!json_is_integer(id))
		die("invalid 'id' property in tile\n");

	if (json_is_string(file)) {
		printf("%d|%s|", (int)json_integer_value(id), json_string_value(file));

		if (json_is_integer(delay))
			printf("%d\n", (int)json_integer_value(delay));
		else
			printf("10\n");
	}
}

static void
write_tiledef(const json_t *tile)
{
	const json_t *id = json_object_get(tile, "id");
	const json_t *objectgroup = json_object_get(tile, "objectgroup");
	const json_t *objects = json_object_get(objectgroup, "objects");
	const json_t *first = json_array_get(objects, 0);
	const json_t *x, *y, *w, *h;

	/* Collisions are optional. */
	if (!json_is_object(objectgroup))
		return;

	if (!json_is_integer(id))
		die("invalid 'id' property in tile\n");
	if (!json_is_array(objects))
		die("invalid 'objects' property in tile\n");

	x = json_object_get(first, "x");
	y = json_object_get(first, "y");
	w = json_object_get(first, "width");
	h = json_object_get(first, "height");

	if (!json_is_integer(x) || !json_is_integer(y) ||
	    !json_is_integer(w) || !json_is_integer(h))
		die("invalid collide object in tile description\n");

	printf("%d|%d|%d|%d|%d\n",
	    (int)json_integer_value(id),
	    (int)json_integer_value(x),
	    (int)json_integer_value(y),
	    (int)json_integer_value(w),
	    (int)json_integer_value(h));
}

static void
write_tiledefs(const json_t *tiles)
{
	size_t index;
	json_t *object;

	if (!json_is_array(tiles))
		return;

	puts("tiledefs");

	json_array_foreach(tiles, index, object) {
		if (!json_is_object(object))
			die("tile is not an object\n");

		write_tiledef(object);
	}
}

static void
write_animations(const json_t *tiles)
{
	size_t index;
	json_t *object;

	if (!json_is_array(tiles))
		return;

	puts("animations");

	json_array_foreach(tiles, index, object) {
		if (!json_is_object(object))
			die("tile is not an object\n");

		write_animation(object);
	}
}

int
main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	json_t *document;
	json_error_t error;

	document = json_loadf(stdin, 0, &error);

	if (!document)
		die("%d:%d: %s\n", error.line, error.column, error.text);
	if (!json_is_object(document))
		die("root value isn't an object\n");

	write_dimensions(document);
	write_image(document);
	write_tiledefs(json_object_get(document, "tiles"));
	write_animations(json_object_get(document, "tiles"));

	json_decref(document);
}
