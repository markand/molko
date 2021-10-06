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

#define GREATEST_USE_ABBREVS 0
#include <greatest.h>

#include <core/util.h>

#include <rpg/quest.h>
#include <rpg/save.h>

static struct save db;

static void
clean(void *data)
{
	(void)data;

	save_finish(&db);
	remove("quest.db");
}

static void
setup(void *data)
{
	(void)data;

	if (save_open_path(&db, "quest.db", SAVE_MODE_WRITE) < 0)
		exit(1);
}

GREATEST_TEST
basics_load(void)
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

	GREATEST_ASSERT_EQ(quest_save(&quest, &db), 0);

	/* Reset to inspect. */
	steps[0].percent = steps[1].percent = 0;
	GREATEST_ASSERT_EQ(quest_load(&quest, &db), 0);
	GREATEST_ASSERT_EQ(steps[0].percent, 100);
	GREATEST_ASSERT_EQ(steps[1].percent, 50);

	GREATEST_PASS();
}

GREATEST_SUITE(suite_basics)
{
	GREATEST_SET_SETUP_CB(setup, NULL);
	GREATEST_SET_TEARDOWN_CB(clean, NULL);
	GREATEST_RUN_TEST(basics_load);
}

GREATEST_MAIN_DEFS();

int
main(int argc, char **argv)
{
	GREATEST_MAIN_BEGIN();
	GREATEST_RUN_SUITE(suite_basics);
	GREATEST_MAIN_END();
}
