/*
 * tileset.h -- map tileset definition
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

#ifndef MLK_RPG_TILESET_H
#define MLK_RPG_TILESET_H

/**
 * \file mlk/rpg/tileset.h
 * \brief Map tileset definition
 */

#include <stddef.h>

struct mlk_animation;
struct mlk_sprite;

/**
 * \struct mlk_tileset_collision
 * \brief Describe a tile collision box.
 */
struct mlk_tileset_collision {
	/**
	 * (read-write)
	 *
	 * The sprite cell index.
	 */
	unsigned int id;

	/**
	 * (read-write)
	 *
	 * Beginning of collision box in x.
	 */
	int x;

	/**
	 * (read-write)
	 *
	 * Beginning of collision box in y.
	 */
	int y;

	/**
	 * (read-write)
	 *
	 * Collision box width.
	 */
	unsigned int w;

	/**
	 * (read-write)
	 *
	 * Collision box height.
	 */
	unsigned int h;
};

/**
 * \struct mlk_tileset_animation
 * \brief Animation per tile
 */
struct mlk_tileset_animation {
	/**
	 * (read-write)
	 *
	 * The sprite cell index.
	 */
	unsigned int id;

	/**
	 * (read-write, borrowed)
	 *
	 * Animation to used for this tile.
	 */
	struct mlk_animation *animation;
};

/**
 * \struct mlk_tileset
 * \brief Tileset structure
 */
struct mlk_tileset {
	/**
	 * (read-write, borrowed, optional)
	 *
	 * Array of collision boxes per tile that MUST be order by tile id.
	 */
	struct mlk_tileset_collision *collisions;

	/**
	 * (read-write)
	 *
	 * Number of items in the ::mlk_tileset::collisions array.
	 */
	size_t collisionsz;

	/**
	 * (read-write, borrowed, optional)
	 *
	 * Array of animations per tile that MUST be order by tile id.
	 */
	struct mlk_tileset_animation *animations;

	/**
	 * (read-write)
	 *
	 * Number of items in the ::mlk_tileset::animations array.
	 */
	size_t animationsz;

	/**
	 * (read-write, borrowed)
	 *
	 * Sprite used to render the map.
	 */
	struct mlk_sprite *sprite;
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Tells if the tileset is usable.
 *
 * \param tileset the tileset to check
 * \return non-zero if the tileset structure is usable
 */
int
mlk_tileset_ok(const struct mlk_tileset *tileset);

/**
 * Start tileset animations.
 *
 * \pre tileset != NULL
 * \param tileset the tileset
 */
void
mlk_tileset_start(struct mlk_tileset *tileset);

/**
 * Update the tileset animations.
 *
 * \pre tileset != NULL
 * \param tileset the tileset
 * \param ticks frame ticks
 */
void
mlk_tileset_update(struct mlk_tileset *tileset, unsigned int ticks);

/**
 * Draw a cell row/column into the given position.
 *
 * \pre tileset != NULL
 * \param tileset the tileset
 * \param r the cell row number
 * \param c the cell column number
 * \param x the x coordinate
 * \param y the y coordinate
 * \return 0 on success or an error code on failure
 */
int
mlk_tileset_draw(const struct mlk_tileset *tileset, unsigned int r, unsigned int c, int x, int y);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_RPG_TILESET_H */
