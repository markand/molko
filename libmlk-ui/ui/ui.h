/*
 * ui.h -- libui convenient header
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

#ifndef MOLKO_UI_UI_H
#define MOLKO_UI_UI_H

/**
 * \file ui.h
 * \brief libui convenient header.
 */

#include <stdbool.h>

#include <core/core.h>

CORE_BEGIN_DECLS

/**
 * Initialize the ui library.
 *
 * \return False on errors.
 */
bool
ui_init(void);

/**
 * Close the ui library.
 */
void
ui_finish(void);

CORE_END_DECLS

#endif /* !MOLKO_UI_UI_H */
