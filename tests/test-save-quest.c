/*
 * test-save-quest.c -- test save routines
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

#include <stdio.h>

#include <mlk/core/util.h>

#include <mlk/rpg/quest.h>
#include <mlk/rpg/save.h>

#include <dt.h>

static void
test_basics_load(struct mlk_save *save)
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
		.stepsz = MLK_UTIL_SIZE(steps)
	};

	DT_EQ_INT(quest_save(&quest, save), 0);

	/* Reset to inspect. */
	steps[0].percent = steps[1].percent = 0;
	DT_EQ_INT(quest_load(&quest, save), 0);
	DT_EQ_INT(steps[0].percent, 100);
	DT_EQ_INT(steps[1].percent, 50);
}

int
main(void)
{
	struct mlk_save save;

	if (mlk_save_open_path(&save, "quest.db", MLK_SAVE_MODE_WRITE) < 0)
		return 1;

	DT_RUN(test_basics_load, &save);
	DT_SUMMARY();

	mlk_save_finish(&save);
	remove("quest.db");
}
