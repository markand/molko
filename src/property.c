/*
 * property.c -- key-value properties
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
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "property.h"

struct property *
property_new(const char *key, const char *value)
{
	assert(key && strlen(key) <= PROPERTY_KEY_MAX);
	assert(value && strlen(value) <= PROPERTY_VALUE_MAX);

	struct property *prop;

	prop = ecalloc(1, sizeof (struct property));
	strncpy(prop->key, key, PROPERTY_KEY_MAX);
	strncpy(prop->value, value, PROPERTY_VALUE_MAX);

	return prop;
}

void
property_add(struct property_list *head, struct property *prop)
{
	assert(head);
	assert(prop);

	if (head->next == NULL)
		head->next = prop;
	else
		head->next->next = prop;
}

struct property *
property_find(struct property_list *head, const char *key)
{
	assert(head);
	assert(key);

	for (struct property *it = head->next; it; it = it->next)
		if (strcmp(it->key, key) == 0)
			return it;

	return NULL;
}

void
property_clear(struct property_list *head)
{
	assert(head);

	struct property *it;
	struct property *next;

	for (it = head->next; it && it->next; ) {
		next = it->next;
		free(it);
		it = next;
	}

	head->next = NULL;
}
