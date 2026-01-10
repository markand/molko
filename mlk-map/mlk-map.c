/*
 * mlk-map.c -- convert tiled tiled JSON files into custom files
 *
 * Copyright (c) 2020-2026 David Demelier <markand@malikania.fr>
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
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include <jansson.h>

#include <mlk/util/util.h>

static inline int
is_layer(const char *name)
{
	return strcmp(name, "background") == 0 ||
	       strcmp(name, "foreground") == 0 ||
	       strcmp(name, "above") == 0 ||
	       strcmp(name, "actions") == 0;
}

static const json_t *
find_property(const json_t *props, const char *which)
{
	const json_t *obj, *key, *value;
	size_t index;

	json_array_foreach(props, index, obj) {
		if (!json_is_object(obj))
			continue;

		key = json_object_get(obj, "name");
		value = json_object_get(obj, "value");

		if (json_is_string(key) && value && strcmp(json_string_value(key), which) == 0)
			return value;
	}

	return NULL;
}

static void
write_player_origin(const json_t *props)
{
	const json_t *x, *y;

	x = find_property(props, "player-origin-x");
	y = find_property(props, "player-origin-y");

	if (!x || !json_is_integer(x) ||
	    !y || !json_is_integer(y))
		return;

	printf("player-origin|%d|%d\n",
	    (int)json_integer_value(x),
	    (int)json_integer_value(y));
}

static void
write_player_sprite(const json_t *props)
{
	const json_t *sprite, *w, *h;

	sprite = find_property(props, "player-sprite");
	w = find_property(props, "player-sprite-w");
	h = find_property(props, "player-sprite-h");

	if (!sprite || !json_is_string(sprite) ||
	    !w      || !json_is_integer(w)     ||
	    !h      || !json_is_integer(h))
		return;

	printf("player-sprite|%d|%d|%s\n",
	    (int)json_integer_value(w),
	    (int)json_integer_value(h),
	    json_string_value(sprite)
	);
}

static void
write_properties(const json_t *props)
{
	write_player_origin(props);
	write_player_sprite(props);
}

static void
write_dimensions(const json_t *document)
{
	json_t *width, *height;

	width = json_object_get(document, "width");
	height = json_object_get(document, "height");

	if (!width || !json_is_integer(width))
		mlk_util_die("missing 'width' property\n");
	if (!height || !json_is_integer(height))
		mlk_util_die("missing 'height' property\n");

	printf("columns|%d\n", (int)json_integer_value(width));
	printf("rows|%d\n", (int)json_integer_value(height));
}

static void
write_object(const json_t *object)
{
	assert(json_is_object(object));

	const json_t *x, *y, *width, *height, *props, *exec, *block;

	x = json_object_get(object, "x");
	y = json_object_get(object, "y");
	width = json_object_get(object, "width");
	height = json_object_get(object, "height");
	props = json_object_get(object, "properties");

	if (!x || !json_is_number(x))
		mlk_util_die("invalid 'x' property in object\n");
	if (!y || !json_is_number(y))
		mlk_util_die("invalid 'y' property in object\n");
	if (!width || !json_is_number(width))
		mlk_util_die("invalid 'width' property in object\n");
	if (!height || !json_is_number(height))
		mlk_util_die("invalid 'height' property in object\n");

	/* This is optional and set to 0 if not present. */
	block = find_property(props, "block");

	/* In tiled, those properties are float but we only use ints in MA */
	printf("%d|%d|%d|%d|%d",
	    (int)json_integer_value(x),
	    (int)json_integer_value(y),
	    (int)json_integer_value(width),
	    (int)json_integer_value(height),
	    (int)json_is_true(block)
	);

	if ((exec = find_property(props, "exec")) && json_is_string(exec))
		printf("|%s", json_string_value(exec));

	printf("\n");
}

static void
write_layer(const json_t *layer)
{
	json_t *objects, *data, *name, *tile, *object;
	size_t index;

	objects = json_object_get(layer, "objects");
	data = json_object_get(layer, "data");
	name = json_object_get(layer, "name");

	if (!name || !json_is_string(name))
		mlk_util_die("invalid 'name' property in layer");
	if (!is_layer(json_string_value(name)))
		mlk_util_die("invalid 'name' layer: %s\n", json_string_value(name));

	printf("layer|%s\n", json_string_value(name));

	/* Only foreground/background have 'data' property */
	if (json_is_array(data)) {
		json_array_foreach(data, index, tile) {
			if (!json_is_integer(tile))
				mlk_util_die("invalid 'data' property in layer\n");

			printf("%d\n", (int)json_integer_value(tile));
		}
	}

	/* Only objects has 'objects' property */
	if (json_is_array(objects)) {
		json_array_foreach(objects, index, object) {
			if (!json_is_object(object))
				mlk_util_die("invalid 'objects' property in layer\n");

			write_object(object);
		}
	}
}

static void
write_layers(const json_t *layers)
{
	size_t index;
	json_t *layer;

	if (!layers)
		return;

	json_array_foreach(layers, index, layer) {
		if (!json_is_object(layer))
			mlk_util_die("layer is not an object\n");

		write_layer(layer);
	}
}

static void
write_tileset(const json_t *tilesets)
{
	char path[MLK_PATH_MAX], *ext;
	const json_t *tileset, *source;

	if (json_array_size(tilesets) != 1)
		mlk_util_die("map must contain exactly one tileset");

	tileset = json_array_get(tilesets, 0);
	source = json_object_get(tileset, "source");

	if (!json_is_string(source))
		mlk_util_die("invalid 'source' property in tileset\n");

	/* We need to replace the .json extension to .tileset. */
	snprintf(path, sizeof (path), "%s", json_string_value(source));

	if (!(ext = strstr(path, ".json")))
		mlk_util_die("could not determine tileset extension");

	*ext = '\0';

	printf("tileset|%s.tileset\n", path);
}

int
main(void)
{
	json_t *document;
	json_error_t error;

	document = json_loadf(stdin, 0, &error);

	if (!document)
		mlk_util_die("%d:%d: %s\n", error.line, error.column, error.text);

	write_properties(json_object_get(document, "properties"));
	write_dimensions(document);
	write_layers(json_object_get(document, "layers"));
	write_tileset(json_object_get(document, "tilesets"));

	json_decref(document);
}
