/*
 * walksprite.h -- sprite designed for walking entities
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

#ifndef MLK_RPG_WALKSPRITE_H
#define MLK_RPG_WALKSPRITE_H

struct mlk_sprite;

/**
 * \brief Sprite designed for walking entities.
 *
 * This structure works with sprite images that are defined as using the
 * following conventions:
 *
 * ```
 * 7   0   1
 *   ↖ ↑ ↗
 * 6 ←   → 2
 *   ↙ ↓ ↘
 * 5   4   3
 * ```
 *
 * Where numbers define row in the sprite according to the character
 * orientation. In other terms, your image sprite should look like this:
 *
 * ```
 * row columns in your image
 * ---|---------------------
 *  0 | ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
 *  1 | ↗↗↗↗↗↗↗↗↗↗↗↗↗↗↗↗↗↗↗↗
 *  2 | →→→→→→→→→→→→→→→→→→→→
 *  3 | ↘↘↘↘↘↘↘↘↘↘↘↘↘↘↘↘↘↘↘↘
 *  4 | ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
 *  5 | ↙↙↙↙↙↙↙↙↙↙↙↙↙↙↙↙↙↙↙↙
 *  6 | ←←←←←←←←←←←←←←←←←←←←
 *  7 | ↖↖↖↖↖↖↖↖↖↖↖↖↖↖↖↖↖↖↖↖
 * ```
 */
struct walksprite {
	struct mlk_sprite *sprite;
	unsigned int delay;
	unsigned int index;
	unsigned int elapsed;
};

#if defined(__cplusplus)
extern "C" {
#endif

void
walksprite_init(struct walksprite *, struct mlk_sprite *, unsigned int);

void
walksprite_reset(struct walksprite *);

void
walksprite_update(struct walksprite *, unsigned int);

void
walksprite_draw(const struct walksprite *, unsigned int, int, int);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_RPG_WALKSPRITE_H */
