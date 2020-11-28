/*
 * save.h -- save functions
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

#ifndef MOLKO_CORE_SAVE_H
#define MOLKO_CORE_SAVE_H

/**
 * \file save.h
 * \brief Save functions.
 *
 * This module provides several functions to save the game data into a database.
 *
 * Database can be opened in read only mode (\ref SAVE_MODE_READ) which will
 * return an error if not present or write mode (\ref SAVE_MODE_WRITE) which
 * will create and initialize a database if not present on disk.
 */

#include <stdbool.h>
#include <time.h>

/**
 * \brief Max property key.
 */
#define SAVE_PROPERTY_KEY_MAX   (64)

/**
 * \brief Max property value.
 */
#define SAVE_PROPERTY_VALUE_MAX (1024)

/**
 * \brief Save database handle.
 */
struct save {
	time_t created; /*!< (-) Date when the save was created. */
	time_t updated; /*!< (-) Date when it was last updated. */
	void *handle;   /*!< (*) Private handle. */
};

/**
 * \brief Open mode.
 */
enum save_mode {
	SAVE_MODE_READ, /*!< Only try to read (no creation). */
	SAVE_MODE_WRITE /*!< Open for both reading and writing */
};

/**
 * \brief Mapping for the property table.
 */
struct save_property {
	/**
	 * (+) Property key to set.
	 */
	char key[SAVE_PROPERTY_KEY_MAX + 1];

	/**
	 * (+) Property value to set.
	 */
	char value[SAVE_PROPERTY_VALUE_MAX + 1];
};

/**
 * Open a database by index.
 *
 * This function use the preferred path to store local files under the user
 * home directory. The parameter idx specifies the save slot to use.
 *
 * \pre db != NULL
 * \param db the database to initialize
 * \param idx the save slot
 * \param mode the mode
 * \return False on error.
 */
bool
save_open(struct save *db, unsigned int idx, enum save_mode mode);

/**
 * Open the save slot specified by path.
 *
 * \pre db != NULL
 * \pre path != NULL
 * \param db the database to initialize
 * \param path the path to the save slot
 * \param mode the mode
 * \return False on error.
 */
bool
save_open_path(struct save *db, const char *path, enum save_mode mode);

/**
 * Sets an arbitrary property.
 *
 * If the property already exists, replace it.
 *
 * \pre db != NULL
 * \pre prop != NULL
 * \param db the database
 * \param prop the property to set
 * \return False on error.
 */
bool
save_set_property(struct save *db, const struct save_property *prop);

/**
 * Get a property.
 *
 * Call this function by setting prop->key to the desired key to get.
 *
 * \pre db != NULL
 * \param db the database
 * \param prop the property to retrieve
 * \return False on error and prop->value is left untouched
 */
bool
save_get_property(struct save *db, struct save_property *prop);

/**
 * Remove a property.
 *
 * \pre db != NULL
 * \pre prop != NULL
 * \param db the database
 * \param prop the property to remove
 * \return false on error
 */
bool
save_remove_property(struct save *db, const struct save_property *prop);

/**
 * Close the save slot.
 *
 * \pre db != NULL
 * \param db the database to close
 */
void
save_finish(struct save *db);

#endif /* !MOLKO_CORE_SAVE_H */
