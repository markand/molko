/*
 * loader-file_p.h -- resource loader for map/tileset
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

#ifndef MLK_RPG_LOADER_FILE_P_H
#define MLK_RPG_LOADER_FILE_P_H

struct mlk_animation;
struct mlk_sprite;
struct mlk_texture;

struct mlk__loader_file;

struct mlk__loader_file *
mlk__loader_file_new(const char *path);

const char *
mlk__loader_file_directory(const struct mlk__loader_file *loader);

struct mlk_texture *
mlk__loader_file_texture_open(struct mlk__loader_file *loader, const char *ident);

struct mlk_sprite *
mlk__loader_file_sprite_new(struct mlk__loader_file *loader);

struct mlk_animation *
mlk__loader_file_animation_new(struct mlk__loader_file *loader);

void
mlk__loader_file_clear(struct mlk__loader_file *loader);

void
mlk__loader_file_free(struct mlk__loader_file *loader);

#endif /* !MLK_RPG_LOADER_FILE_P_H */
