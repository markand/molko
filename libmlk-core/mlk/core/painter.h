/*
 * painter.h -- basic drawing routines
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

#ifndef MLK_CORE_PAINTER_H
#define MLK_CORE_PAINTER_H

/**
 * \file painter.h
 * \brief Basic drawing routines
 *
 * Primitive drawing routines.
 *
 * Drawing using this module operates on a texture which can be the window
 * itself or a custom off-screen texture. To select a texture use
 * ::mlk_painter_set_target with a texture, to resume drawing to the window
 * pass NULL as argument.
 *
 * The convenient macros ::MLK_PAINTER_BEGIN and ::MLK_PAINTER_END can be used
 * to create a scoped block for a given texture and switch back to the previous
 * one.
 *
 * ## Drawing in the main loop
 *
 * In your game loop, you should always clear, draw and present the screen in
 * this order.
 *
 * ```c
 * mlk_painter_set_color(0x00000000);
 * mlk_painter_clear();
 * // use your favorite draw functions.
 * mlk_painter_present();
 * ```
 *
 * ## Switching between textures
 *
 * This example demonstrates how to draw to a texture and back to the screen.
 *
 * ```c
 * struct mlk_texture texture;
 *
 * mlk_texture_new(&texture, 100, 100);
 *
 * // Switch to this texture for next rendering functions.
 * MLK_PAINTER_BEGIN(&texture);
 * mlk_painter_set_color(0xffffffff);
 * mlk_painter_clear();
 * mlk_painter_set_color(0xff00ffff);
 * mlk_painter_draw_rectangle(10, 10, 80, 80);
 * MLK_PAINTER_END();
 *
 * // Now, drawing this texture is done on the last texture target.
 * mlk_texture_draw(&texture, 10, 10);
 * ```
 */

/**
 * Store the current texture in a temporary variable and switch to the
 * rendering target provided as argument.
 *
 * \param tex the texture to use
 */
#define MLK_PAINTER_BEGIN(tex)                                          \
do {                                                                    \
        struct mlk_texture *__current_texture__;                        \
                                                                        \
        __current_texture__ = mlk_painter_get_target();                 \
        mlk_painter_set_target((tex))

/**
 * Pop the current rendering texture and restore the previous one.
 */
#define MLK_PAINTER_END()                                               \
        mlk_painter_set_target(__current_texture__);                    \
} while (0)

struct mlk_texture;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Get the current rendering texture target.
 *
 * \return the current target (or NULL in case of direct window rendering)
 */
struct mlk_texture *
mlk_painter_get_target(void);

/**
 * Change the rendering target.
 *
 * \param texture the texture (or NULL for rendering to the window)
 */
void
mlk_painter_set_target(struct mlk_texture *texture);

/**
 * Get the current rendering color.
 *
 * \return the color
 */
unsigned long
mlk_painter_get_color(void);

/**
 * Set the rendering color.
 *
 * \param color the color
 */
void
mlk_painter_set_color(unsigned long color);

/**
 * Draw a line.
 *
 * \param x1 start x coordinate
 * \param y1 start y coordinate
 * \param x2 end x coordinate
 * \param y2 end y coordinate
 */
void
mlk_painter_draw_line(int x1, int y1, int x2, int y2);

/**
 * Draw a unique point.
 *
 * \param x the x coordinate
 * \param y the y coordinate
 */
void
mlk_painter_draw_point(int x, int y);

/**
 * Draw a rectangle.
 *
 * \param x the x coordinate
 * \param y the y coordinate
 * \param w rectangle width
 * \param h rectangle height
 */
void
mlk_painter_draw_rectangle(int x, int y, unsigned int w, unsigned int h);

/**
 * Draw a circle.
 *
 * \param x the x coordinate
 * \param y the y coordinate
 * \param radius circle radius
 */
void
mlk_painter_draw_circle(int x, int y, int radius);

/**
 * Clear the texture with the current rendering color.
 */
void
mlk_painter_clear(void);

/**
 * Present the rendering context.
 *
 * \note This function should be called only once in your main loop.
 */
void
mlk_painter_present(void);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_PAINTER_H */
