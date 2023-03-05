/*
 * example.c -- libmlk-example entry point
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

#include <assert.h>

#include <mlk/core/core.h>
#include <mlk/core/err.h>
#include <mlk/core/window.h>

#include <mlk/ui/ui.h>

#include <mlk/rpg/rpg.h>

#include "example.h"
#include "registry.h"

int
mlk_example_init(const char *name)
{
	assert(name);

	int err;

	if ((err = mlk_core_init("fr.malikania", name)) < 0)
		return err;
	if ((err = mlk_ui_init()) < 0)
		return err;
	if ((err = mlk_rpg_init()) < 0)
		return err;
	if (mlk_window_open(name, MLK_EXAMPLE_W, MLK_EXAMPLE_H) < 0)
		return err;

	mlk_registry_init();

	return 0;
}

void
mlk_example_finish(void)
{
	mlk_window_finish();
	mlk_rpg_finish();
	mlk_ui_finish();
	mlk_core_finish();
}
