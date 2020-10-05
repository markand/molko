/*
 * panic_state.h -- panic state
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

#ifndef MOLKO_PANIC_STATE_H
#define MOLKO_PANIC_STATE_H

/**
 * \file panic_state.h
 * \brief Panic state.
 */

#include <core/state.h>

/**
 * \brief Global panic state structure.
 */
extern struct state panic_state;

/**
 * Call this function as early as possible to be able to use this state even on
 * memory allocation errors.
 *
 * \note You must still initialize the system before.
 * \see \ref sys_init
 * \see \ref window_init
 */
void
panic_state_init(void);

#endif /* !MOLKO_PANIC_STATE_H */
