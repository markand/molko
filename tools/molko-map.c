/*
 * molko-map.c -- convert tiled tiled JSON files into custom files
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
	       strcmp(name, "objects") == 0;
}

static void
write_properties(const json_t *props)
{
	size_t index;
	json_t *prop, *name, *type, *value;

	if (!json_is_array(props))
		return;

	json_array_foreach(props, index, prop) {
		name = json_object_get(prop, "name");
		type = json_object_get(prop, "type");
		value = json_object_get(prop, "value");

		if (!json_is_object(prop) ||
		    !json_is_string(name) ||
		    !json_is_string(type) ||
		    !json_is_string(value))
			die("invalid property\n");

		printf("%s|%s\n", json_string_value(name),
		    json_string_value(value));
	}
}

static void
write_metadata(const json_t *document)
{
	json_t *height = json_object_get(document, "height");
	json_t *width = json_object_get(document, "width");

	if (!height || !json_is_integer(height))
		die("missing 'height' property\n");
	if (!width || !json_is_integer(width))
		die("missing 'width' property\n");

	printf("width|%lld\n", json_integer_value(width));
	printf("height|%lld\n", json_integer_value(height));
}

static void
write_object_property(int id, const json_t *property)
{
	assert(json_is_object(property));

	json_t *name = json_object_get(property, "name");
	json_t *type = json_object_get(property, "type");
	json_t *value = json_object_get(property, "value");

	if (!name || !json_is_string(name))
		die("invalid 'name' property in object");
	if (!type || !json_is_string(type))
		die("invalid 'type' property in object");
	if (!value || !json_is_string(value))
		die("invalid 'value' property in object");

	printf("object-property|%d|%s|%s\n",
	    id,
	    json_string_value(name),
	    json_string_value(value)
	);
}

static void
write_object(const json_t *object)
{
	assert(json_is_object(object));

	json_t *id = json_object_get(object, "id");
	json_t *x = json_object_get(object, "x");
	json_t *y = json_object_get(object, "y");
	json_t *width = json_object_get(object, "width");
	json_t *height = json_object_get(object, "height");
	json_t *type = json_object_get(object, "type");
	json_t *props = json_object_get(object, "properties");

	if (!id || !json_is_integer(id))
		die("invalid 'id' property in object\n");
	if (!x || !json_is_real(x))
		die("invalid 'x' property in object\n");
	if (!y || !json_is_real(y))
		die("invalid 'y' property in object\n");
	if (!width || !json_is_real(width))
		die("invalid 'width' property in object\n");
	if (!height || !json_is_real(height))
		die("invalid 'height' property in object\n");
	if (!type || !json_is_string(type))
		die("invalid 'type' property in object\n");

	/* In tiled, those properties are float but we only use ints in MA */
	printf("object|%lld|%s|%d|%d|%d|%d\n",
	    json_integer_value(id),
	    json_string_value(type),
	    (int)json_real_value(x),
	    (int)json_real_value(y),
	    (int)json_real_value(width),
	    (int)json_real_value(height)
	);

	if (json_is_array(props)) {
		json_t *prop;
		size_t index;

		json_array_foreach(props, index, prop) {
			if (!json_is_object(prop))
				die("invalid property in object\n");

			write_object_property(json_integer_value(id), prop);
		}
	}
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

			printf("%lld\n", json_integer_value(tile));
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
write_tileset(const json_t *tileset)
{
	json_t *image = json_object_get(tileset, "image");

	if (!image || !json_is_string(image))
		die("invalid 'image' property in tileset");

	printf("tileset|%s\n", json_string_value(image));
}

static void
write_tilesets(const json_t *tilesets)
{
	json_t *tileset;
	size_t index;

	if (!json_is_array(tilesets))
		die("invalid 'tilesets' property");

	json_array_foreach(tilesets, index, tileset) {
		if (!json_is_object(tileset))
			die("invalid tileset");

		write_tileset(tileset);
	}
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
	write_metadata(document);
	write_layers(json_object_get(document, "layers"));
	write_tilesets(json_object_get(document, "tilesets"));

	json_decref(document);
}
