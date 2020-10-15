/*
 * map_state.h -- state when player is on a map
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

#ifndef MOLKO_MAP_STATE_H
#define MOLKO_MAP_STATE_H

/**
 * \file map_state.h
 * \brief State when player is on a map.
 * \ingroup states
 */

#include <core/sprite.h>

#include "map.h"

/**
 * \brief Data for the state.
 *
 * Update this structure before switching to this state.
 */
extern struct map_state_data {
	/**
	 * Map properties.
	 */
	struct {
		struct map_data data;   /*!< (+) Map data. */
		struct map map;         /*!< (+) Map object. */
	} map;

	/**
	 * Player position.
	 *
	 * If you adjust this structure, it is strictly encouraged to update
	 * the view as well.
	 */
	struct {
		struct sprite sprite;   /*!< (+) The sprite to use */
		int x;                  /*!< (+) Player position in x */
		int y;                  /*!< (+) Player position in y */
		int angle;              /*!< (+) Player angle (see walksprite) */
	} player;

	/**
	 * Position and size of the view.
	 */
	struct {
		int x;                  /*!< (+) Position in x */
		int y;                  /*!< (+) Position in y */
		unsigned int w;         /*!< (+) View width */
		unsigned int h;         /*!< (+) View height */
	} view;
} map_state_data; /*!< Access to data. */

/**
 * \brief State when player is on a map.
 */
extern struct state map_state;

#endif /* !MOLKO_MAP_STATE_H */
