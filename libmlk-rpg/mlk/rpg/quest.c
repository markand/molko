/*
 * quest.c -- in game quests
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

#include <assert.h>

#include <assets/sql/quest-remove.h>
#include <assets/sql/quest-save.h>
#include <assets/sql/quest-step-load.h>
#include <assets/sql/quest-step-save.h>

#include "quest.h"
#include "save.h"

int
quest_save(struct quest *q, struct mlk_save *s)
{
	assert(q);
	assert(s);

	const struct quest_step *step;

	if (mlk_save_tx_begin(s) < 0)
		return -1;
	if (mlk_save_exec(s, (const char *)assets_sql_quest_save, "s", q->name) < 0) {
		mlk_save_tx_rollback(s);
		return -1;
	}

	for (size_t i = 0; i < q->stepsz; ++i) {
		step = &q->steps[i];

		if (mlk_save_exec(s, (const char *)assets_sql_quest_step_save, "ssi", q->name, step->name, step->percent) < 0) {
			mlk_save_tx_rollback(s);
			return -1;
		}
	}

	return mlk_save_tx_commit(s);
}

int
quest_load(struct quest *q, struct mlk_save *s)
{
	assert(q);
	assert(s);

	struct mlk_save_stmt stmt;
	struct quest_step *step;

	for (size_t i = 0; i < q->stepsz; ++i) {
		step = &q->steps[i];

		if (mlk_save_stmt_init(&stmt, s, (const char *)assets_sql_quest_step_load, "s", step->name))
			return -1;
		if (mlk_save_stmt_next(&stmt, "i", &step->percent) < 0) {
			mlk_save_stmt_finish(&stmt);
			return -1;
		}

		mlk_save_stmt_finish(&stmt);
	}

	return 0;
}
