/*
 * state.h -- abstract state
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

#ifndef MLK_CORE_STATE_H
#define MLK_CORE_STATE_H

union mlk_event;

struct mlk_state {
	void *data;
	void (*start)(struct mlk_state *);
	void (*handle)(struct mlk_state *, const union mlk_event *);
	void (*update)(struct mlk_state *, unsigned int);
	void (*draw)(struct mlk_state *);
	void (*suspend)(struct mlk_state *);
	void (*resume)(struct mlk_state *);
	void (*end)(struct mlk_state *);
	void (*finish)(struct mlk_state *);
};

#if defined(__cplusplus)
extern "C" {
#endif

void
mlk_state_start(struct mlk_state *);

void
mlk_state_handle(struct mlk_state *, const union mlk_event *);

void
mlk_state_update(struct mlk_state *, unsigned int);

void
mlk_state_draw(struct mlk_state *);

void
mlk_state_suspend(struct mlk_state *);

void
mlk_state_resume(struct mlk_state *);

void
mlk_state_end(struct mlk_state *);

void
mlk_state_finish(struct mlk_state *);

#if defined(__cplusplus)
}
#endif

#endif /* !MLK_CORE_STATE_H */
