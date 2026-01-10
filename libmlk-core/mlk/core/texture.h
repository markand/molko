/*
 * texture.h -- basic texture management
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

#ifndef MLK_CORE_TEXTURE_H
#define MLK_CORE_TEXTURE_H

/**
 * \file texture.h
 * \brief Basic texture management
 */

/**
 * \struct mlk_texture
 * \brief Texture structure
 */
struct mlk_texture {
	/**
	 * (read-only)
	 *
	 * Texture's width.
	 */
	unsigned int w;

	/**
	 * (read-only)
	 *
	 * Texture's height.
	 */
	unsigned int h;

	/** \cond MLK_PRIVATE_DECLS */
	void *handle;
	/** \endcond MLK_PRIVATE_DECLS */
};

/**
 * \enum mlk_texture_blend
 * \brief Controls how to blend alpha/colors.
 */
enum mlk_texture_blend {
	/**
	 * No blending.
	 */
	MLK_TEXTURE_BLEND_NONE,

	/**
	 * Alpha blending.
	 *
	 * ```
	 * RGB = (SRC_RGB * SRC_A) + (DST_RGB * (1 - SRC_A))
	 * A   = SRC_A + (DST_A * (1 - SRC_A))
	 * ```
	 */
	MLK_TEXTURE_BLEND_BLEND,

	/**
	 * Additive blending.
	 *
	 * ```
	 * RGB = (SRC_RGB * SRC_A) + DST_RGB
	 * A   = A
	 * ```
	 */
	MLK_TEXTURE_BLEND_ADD,

	/**
	 * Color modulation.
	 *
	 * ```
	 * RGB = SRC_RGB * DST_RGB
	 * A   = A
	 * ```
	 */
	MLK_TEXTURE_BLEND_MODULATE,

	/**
	 * Unused sentinel value.
	 */
	MLK_TEXTURE_BLEND_LAST
};

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Create a new texture with the given dimensions.
 *
 * \pre texture != NULL
 * \pre w > 0 && h > 0
 * \param texture the texture to initialize
 * \param w the texture width
 * \param h the texture height
 * \return 0 on success or an error code on failure
 */
int
mlk_texture_init(struct mlk_texture *texture, unsigned int w, unsigned int h);

/**
 * Change color/alpha blending mode.
 *
 * \pre texture != NULL
 * \param texture the texture to update
 * \param blend the new blend mode
 * \return 0 on success or an error code on failure
 */
int
mlk_texture_set_blend_mode(struct mlk_texture *texture, enum mlk_texture_blend blend);

/**
 * Change alpha value.
 *
 * \pre texture != NULL
 * \pre alpha >= 0 && alpha <= 255
 * \param texture the texture to update
 * \param alpha transparency value
 * \return 0 on success or an error code on failure
 */
int
mlk_texture_set_alpha_mod(struct mlk_texture *texture, unsigned int alpha);

/**
 * Change color modulation.
 *
 * \pre texture != NULL
 * \param texture the texture to update
 * \param color the color modulation
 * \return 0 on success or an error code on failure
 */
int
mlk_texture_set_color_mod(struct mlk_texture *texture, unsigned long color);

/**
 * Draw the entire texture at the given location.
 *
 * \pre texture != NULL
 * \param texture the texture to draw
 * \param x the x coordinate
 * \param y the y coordinate
 * \return 0 on success or an error code on failure
 */
int
mlk_texture_draw(const struct mlk_texture *texture, int x, int y);

/**
 * Scale a clip of the texture at the given rectangle destination, applying an
 * optional rotation.
 *
 * \pre texture != NULL
 * \param texture the texture to scale
 * \param src_x texture x coordinate
 * \param src_y texture y coordinate
 * \param src_w texture width to clip
 * \param src_h texture height to clip
 * \param dst_x destination rectangle x coordinate
 * \param dst_y destination rectangle y coordinate
 * \param dst_w destination rectangle width
 * \param dst_h destination rectangle height
 * \param angle optional angle to apply
 * \return 0 on success or an error code on failure
 */
int
mlk_texture_scale(const struct mlk_texture *texture,
                  int src_x,
                  int src_y,
                  unsigned src_w,
                  unsigned src_h,
                  int dst_x,
                  int dst_y,
                  unsigned dst_w,
                  unsigned dst_h,
                  double angle);

/**
 * Destroy the texture.
 *
 * \pre texture != NULL
 * \param texture the texture to destroy
 */
void
mlk_texture_finish(struct mlk_texture *texture);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_TEXTURE_H */
