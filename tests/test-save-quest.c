/*
 * test-save-quest.c -- test save routines
 *
 * Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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

#include <stdio.h>

#include <rexo.h>

#include <core/util.h>

#include <rpg/quest.h>
#include <rpg/save.h>

RX_SET_UP(setup)
{
	if (save_open_path(RX_DATA, "quest.db", SAVE_MODE_WRITE) < 0)
		return RX_ERROR;

	return RX_SUCCESS;
}

RX_TEAR_DOWN(teardown)
{
	save_finish(RX_DATA);
	remove("quest.db");
}

RX_FIXTURE(basics_fixture, struct save, .set_up = setup, .tear_down = teardown);

RX_TEST_CASE(basics, load, .fixture = basics_fixture)
{
	struct quest_step steps[] = {
		{
			.name = "0001.01-kill-ten-moko",
			.description = "Kill 10 mokos de las llanuras.",
			.percent = 100
		},
		{
			.name = "0001.02-bring-beer-to-molko",
			.description = "Bring beer to Molko.",
			.percent = 50
		}
	};
	struct quest quest = {
		.name = "0001-getting-started",
		.description = "Initial quest for adventurer.",
		.steps = steps,
		.stepsz = UTIL_SIZE(steps)
	};
	struct save *save = RX_DATA;

	RX_INT_REQUIRE_EQUAL(quest_save(&quest, save), 0);

	/* Reset to inspect. */
	steps[0].percent = steps[1].percent = 0;
	RX_INT_REQUIRE_EQUAL(quest_load(&quest, save), 0);
	RX_INT_REQUIRE_EQUAL(steps[0].percent, 100);
	RX_INT_REQUIRE_EQUAL(steps[1].percent, 50);
}

int
main(int argc, char **argv)
{
	return rx_main(0, NULL, argc, (const char **)argv) == RX_SUCCESS ? 0 : 1;
}
