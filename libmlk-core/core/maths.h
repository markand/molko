/*
 * maths.h -- basic maths
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

#ifndef MOLKO_CORE_MATHS_H
#define MOLKO_CORE_MATHS_H

#include <stdbool.h>

/**
 * \file maths.h
 * \brief Basic maths.
 */

/**
 * Tells if a coordinate is within a rectangle region.
 *
 * \param x the region start
 * \param y the region start
 * \param w the region width
 * \param h the region height
 * \param px the point to test
 * \param py the point to test
 * \return True if within the region
 */
bool
maths_is_boxed(int x, int y, unsigned int w, unsigned int h, int px, int py);

#endif /* !MOLKO_CORE_MATHS_H */
