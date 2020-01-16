/*
 * property.h -- key-value properties
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

#ifndef MOLKO_PROPERTY_H
#define MOLKO_PROPERTY_H

/**
 * \file property.h
 * \brief Key-value properties.
 */

/**
 * Maximum property key length.
 */
#define PROPERTY_KEY_MAX        32

/**
 * Maximum property value length.
 */
#define PROPERTY_VALUE_MAX      128

/**
 * \brief Key-value properties.
 *
 * This structure is implemented as linked-lists.
 */
struct property {
	char key[PROPERTY_KEY_MAX + 1];         /*!< (RW) Property key */
	char value[PROPERTY_VALUE_MAX + 1];     /*!< (RW) Property value */
	struct property *next;                  /*!< (RO) Pointer to next property */
};

/**
 * \brief The linked list of properties.
 */
struct property_list {
	struct property *next;                  /*!< (RO) Property head */
};

/**
 * Create a new property.
 *
 * \pre key != NULL
 * \pre value != NULL
 * \param key the property key
 * \param value the property value
 * \return a non-NULL property
 * \note The property must be deallocated with free(3)
 * \see property_clear
 */
struct property *
property_new(const char *key, const char *value);

/**
 * Add a property to the linked-list specified by head.
 *
 * \pre head != NULL
 * \pre prop != NULL
 * \param head the linked-list
 * \param prop the property
 */
void
property_add(struct property_list *head, struct property *prop);

/**
 * Try to find a property by key.
 *
 * \pre head != NULL
 * \pre key != NULL
 * \param head the linked-list
 * \param key the property key
 * \return the property or NULL on failure
 */
struct property *
property_find(struct property_list *head, const char *key);

/**
 * Remove and free all properties.
 *
 * \pre head != NULL
 * \param head the linked-list
 * \warning All properties must be allocated on the heap.
 */
void
property_clear(struct property_list *head);

#endif /* !MOLKO_PROPERTY_H */
