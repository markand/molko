/*
 * mlk-tileset.c -- convert tiled tilesets JSON files into custom files
 *
 * Copyright (c) 2020-2023 David Demelier <markand@malikania.fr>
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
#include <string.h>

#include <jansson.h>

#include <mlk/util/util.h>

static void
write_dimensions(const json_t *document)
{
	const json_t *tilewidth, *tileheight;

	tilewidth = json_object_get(document, "tilewidth");
	tileheight = json_object_get(document, "tileheight");

	if (!json_is_integer(tilewidth))
		mlk_util_die("invalid 'tilewidth' property\n");
	if (!json_is_integer(tileheight))
		mlk_util_die("invalid 'tileheight' property\n");

	printf("tilewidth|%u\n", (unsigned int)json_integer_value(tilewidth));
	printf("tileheight|%u\n", (unsigned int)json_integer_value(tileheight));
}

static void
write_image(const json_t *document)
{
	const json_t *image;

	image = json_object_get(document, "image");

	if (!json_is_string(image))
		mlk_util_die("invalid 'image' property\n");

	printf("image|%s\n", json_string_value(image));
}

static const json_t *
find_property_value(const json_t *array, const char *prop)
{
	const json_t *obj, *name;
	size_t i;

	json_array_foreach(array, i, obj) {
		name = json_object_get(obj, "name");

		if (!name || !json_is_string(name))
			mlk_util_die("invalid property object\n");

		if (strcmp(json_string_value(name), prop) == 0)
			return json_object_get(obj, "value");
	}

	return NULL;
}

static void
write_animation(const json_t *tile)
{
	const json_t *id, *properties, *file, *delay;

	id = json_object_get(tile, "id");
	properties = json_object_get(tile, "properties");
	file = find_property_value(properties, "animation-file");
	delay = find_property_value(properties, "animation-delay");

	/* Animations are completely optional. */
	if (!json_is_array(properties))
		return;

	if (!json_is_integer(id))
		mlk_util_die("invalid 'id' property in tile\n");

	if (json_is_string(file)) {
		printf("%d|%s|", (int)json_integer_value(id), json_string_value(file));

		if (json_is_integer(delay))
			printf("%d\n", (int)json_integer_value(delay));
		else
			printf("10\n");
	}
}

static void
write_collision(const json_t *tile)
{
	const json_t *id, *objectgroup, *objects, *first, *x, *y, *w, *h;

	id = json_object_get(tile, "id");
	objectgroup = json_object_get(tile, "objectgroup");
	objects = json_object_get(objectgroup, "objects");
	first = json_array_get(objects, 0);

	/* Collisions are optional. */
	if (!json_is_object(objectgroup))
		return;

	if (!json_is_integer(id))
		mlk_util_die("invalid 'id' property in tile\n");
	if (!json_is_array(objects))
		mlk_util_die("invalid 'objects' property in tile\n");

	x = json_object_get(first, "x");
	y = json_object_get(first, "y");
	w = json_object_get(first, "width");
	h = json_object_get(first, "height");

	if (!json_is_integer(x) || !json_is_integer(y) ||
	    !json_is_integer(w) || !json_is_integer(h))
		mlk_util_die("invalid collide object in tile description\n");

	printf("%d|%d|%d|%d|%d\n",
	    (int)json_integer_value(id),
	    (int)json_integer_value(x),
	    (int)json_integer_value(y),
	    (int)json_integer_value(w),
	    (int)json_integer_value(h));
}

static void
write_collisions(const json_t *tiles)
{
	size_t index;
	json_t *object;

	if (!json_is_array(tiles))
		return;

	printf("collisions\n");

	json_array_foreach(tiles, index, object) {
		if (!json_is_object(object))
			mlk_util_die("tile is not an object\n");

		write_collision(object);
	}
}

static void
write_animations(const json_t *tiles)
{
	size_t index;
	json_t *object;

	if (!json_is_array(tiles))
		return;

	printf("animations\n");

	json_array_foreach(tiles, index, object) {
		if (!json_is_object(object))
			mlk_util_die("tile is not an object\n");

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
		mlk_util_die("%d:%d: %s\n", error.line, error.column, error.text);
	if (!json_is_object(document))
		mlk_util_die("root value isn't an object\n");

	write_dimensions(document);
	write_image(document);
	write_collisions(json_object_get(document, "tiles"));
	write_animations(json_object_get(document, "tiles"));

	json_decref(document);
}
