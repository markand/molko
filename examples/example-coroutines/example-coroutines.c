/*
 * example-coroutines.c -- example on coroutines
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

#include <mlk/core/game.h>
#include <mlk/core/panic.h>

#include <mlk/example/example.h>

#include "app.h"

static void
init(void)
{
	if (mlk_example_init("example-coroutines") < 0)
		mlk_panic();
}

static void
run(void)
{
	app_run();
}

static void
quit(void)
{
	mlk_example_finish();
}

int
main(int, char **)
{
	init();
	run();
	quit();

	return 0;
}
