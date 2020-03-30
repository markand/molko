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

#include "map.h"
#include "sprite.h"

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
		struct map_data data;   /*!< (RW) Map data. */
		struct map map;         /*!< (RW) Map object. */
	} map;

	/**
	 * Player position.
	 *
	 * If you adjust this structure, it is strictly encouraged to update
	 * the view as well.
	 */
	struct {
		struct sprite sprite;   /*!< (RW) The sprite to use */
		int x;                  /*!< (RO) Player position in x */
		int y;                  /*!< (RO) Player position in y */
		int angle;              /*!< (RO) Player angle (see walksprite) */
	} player;

	/**
	 * Position and size of the view.
	 */
	struct {
		int x;                  /*!< (RW) Position in x */
		int y;                  /*!< (RW) Position in y */
		unsigned int w;         /*!< (RO) View width */
		unsigned int h;         /*!< (RO) View height */
	} view;
} map_state_data; /*!< Access to data. */

/**
 * \brief State when player is on a map.
 */
extern struct state map_state;

#endif /* !MOLKO_MAP_STATE_H */
