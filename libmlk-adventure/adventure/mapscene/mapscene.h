/*
 * mapscene.h -- map scene loader with actions
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

#ifndef MOLKO_ADVENTURE_MAPSCENE_MAPSCENE_H
#define MOLKO_ADVENTURE_MAPSCENE_MAPSCENE_H

#include <core/core.h>

struct map;

CORE_BEGIN_DECLS

void
mapscene_load_action(struct map *, int, int, int, int, const char *);

void
mapscene_load(struct map *);

CORE_END_DECLS

#endif /* !MOLKO_ADVENTURE_MAPSCENE_MAPSCENE_H */
