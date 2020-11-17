/*
 * main.c -- convert tiled tiled JSON files into custom files
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

#define _XOPEN_SOURCE 700
#include <assert.h>
#include <libgen.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
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

static bool
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
	json_t *value;
	size_t index;

	json_array_foreach(props, index, value) {
		if (!json_is_object(value))
			continue;

		const json_t *key = json_object_get(value, "name");

		if (json_is_string(key) && strcmp(json_string_value(key), which) == 0)
			return value;
	}

	return NULL;
}

static const json_t *
find_action_exec(const json_t *props)
{
	assert(json_is_array(props));

	json_t *prop, *name, *value;
	size_t i;

	json_array_foreach(props, i, prop) {
		if (!json_is_object(prop))
			die("invalid property in object\n");

		name = json_object_get(prop, "name");
		value = json_object_get(prop, "value");

		if (!name || !json_is_string(name))
			die("invalid 'name' property in object properties");
		if (!value || !json_is_string(value))
			die("invalid 'value' property in object properties");

		if (strcmp(json_string_value(name), "exec") == 0)
			return value;
	}

	return NULL;
}

static void
write_title(const json_t *props)
{
	const json_t *prop_title = find_property(props, "title");

	if (!prop_title)
		return;

	const json_t *title = json_object_get(prop_title, "value");

	if (title && json_is_string(title))
		printf("title|%s\n", json_string_value(title));
}

static void
write_origin(const json_t *props)
{
	const json_t *prop_origin_x = find_property(props, "origin-x");
	const json_t *prop_origin_y = find_property(props, "origin-y");

	if (!prop_origin_x || !prop_origin_y)
		return;

	const json_t *origin_x = json_object_get(prop_origin_x, "value");
	const json_t *origin_y = json_object_get(prop_origin_y, "value");

	if (!origin_x || !json_is_integer(origin_x) ||
	    !origin_y || !json_is_integer(origin_y))
		return;

	printf("origin|%d|%d\n",
	    (int)json_integer_value(origin_x),
	    (int)json_integer_value(origin_y));
}

static void
write_properties(const json_t *props)
{
	write_title(props);
	write_origin(props);
}

static void
write_dimensions(const json_t *document)
{
	json_t *width = json_object_get(document, "width");
	json_t *height = json_object_get(document, "height");

	if (!width || !json_is_integer(width))
		die("missing 'width' property\n");
	if (!height || !json_is_integer(height))
		die("missing 'height' property\n");

	printf("columns|%d\n", (int)json_integer_value(width));
	printf("rows|%d\n", (int)json_integer_value(height));
}

static void
write_object(const json_t *object)
{
	assert(json_is_object(object));

	json_t *x = json_object_get(object, "x");
	json_t *y = json_object_get(object, "y");
	json_t *width = json_object_get(object, "width");
	json_t *height = json_object_get(object, "height");
	json_t *props = json_object_get(object, "properties");
	const json_t *exec;

	if (!x || !json_is_number(x))
		die("invalid 'x' property in object\n");
	if (!y || !json_is_number(y))
		die("invalid 'y' property in object\n");
	if (!width || !json_is_number(width))
		die("invalid 'width' property in object\n");
	if (!height || !json_is_number(height))
		die("invalid 'height' property in object\n");

	/* In tiled, those properties are float but we only use ints in MA */
	printf("%d|%d|%d|%d|",
	    (int)json_integer_value(x),
	    (int)json_integer_value(y),
	    (int)json_integer_value(width),
	    (int)json_integer_value(height)
	);

	if ((exec = find_action_exec(props)))
		printf("%s", json_string_value(exec));

	printf("\n");
}

static void
write_layer(const json_t *layer)
{
	json_t *objects = json_object_get(layer, "objects");
	json_t *data = json_object_get(layer, "data");
	json_t *name = json_object_get(layer, "name");
	json_t *tile, *object;
	size_t index;

	if (!name || !json_is_string(name))
		die("invalid 'name' property in layer");
	if (!is_layer(json_string_value(name)))
		die("invalid 'name' layer: %s\n", json_string_value(name));

	printf("layer|%s\n", json_string_value(name));

	/* Only foreground/background have 'data' property */
	if (json_is_array(data)) {
		json_array_foreach(data, index, tile) {
			if (!json_is_integer(tile))
				die("invalid 'data' property in layer\n");

			printf("%d\n", (int)json_integer_value(tile));
		}
	}

	/* Only objects has 'objects' property */
	if (json_is_array(objects)) {
		json_array_foreach(objects, index, object) {
			if (!json_is_object(object))
				die("invalid 'objects' property in layer\n");

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
			die("layer is not an object\n");

		write_layer(layer);
	}
}

static void
write_tileset(const json_t *tilesets)
{
	char path[PATH_MAX];
	char filename[FILENAME_MAX] = {0}, *ext;
	const json_t *tileset, *source;

	if (json_array_size(tilesets) != 1)
		die("map must contain exactly one tileset");

	tileset = json_array_get(tilesets, 0);
	source = json_object_get(tileset, "source");

	if (!json_is_string(source))
		die("invalid 'source' property in tileset\n");

	/* We need to replace the .json extension to .tileset. */
	snprintf(path, sizeof (path), "%s", json_string_value(source));
	snprintf(filename, sizeof (filename), "%s", basename(path));

	if (!(ext = strstr(filename, ".json")))
		die("could not determine tileset extension");

	*ext = '\0';

	printf("tileset|%s.tileset\n", filename);
}

int
main(void)
{
	json_t *document;
	json_error_t error;

	document = json_loadf(stdin, 0, &error);

	if (!document)
		die("%d:%d: %s\n", error.line, error.column, error.text);

	write_properties(json_object_get(document, "properties"));
	write_dimensions(document);
	write_layers(json_object_get(document, "layers"));
	write_tileset(json_object_get(document, "tilesets"));

	json_decref(document);
}
