/*
 * dialog.c -- convenient message wrapper
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

#include <mlk/example/example.h>

#include "dialog.h"

/* Message width is 80% of window width and height is auto computed. */
#define QMW (MLK_EXAMPLE_W * 0.8)

/* Position of each message. */
#define QMX ((MLK_EXAMPLE_W - QMW) / 2)
#define QMY (MLK_EXAMPLE_H * 0.2)

/* Fading time and spacing. */
#define QMD 250
#define QMS 10

static void
handle(struct mlk_action *act, const union mlk_event *ev)
{
	struct dialog *dlg = act->data;

	mlk_message_handle(&dlg->msg, ev);
}

static int
update(struct mlk_action *act, unsigned int ticks)
{
	struct dialog *dlg = act->data;

	return mlk_message_update(&dlg->msg, ticks);
}

static void
draw(struct mlk_action *act)
{
	struct dialog *dlg = act->data;

	mlk_message_draw(&dlg->msg);
}

static void
end(struct mlk_action *act)
{
	struct dialog *dlg = act->data;

	if ((dlg->msg.flags & MLK_MESSAGE_FLAGS_QUESTION) && dlg->response)
		dlg->response(dlg, dlg->msg.index);
}

struct mlk_action *
dialog_init(struct dialog *dlg)
{
	assert(dlg);

	dlg->msg.x = QMX;
	dlg->msg.y = QMY;
	dlg->msg.w = QMW;
	dlg->msg.delay = QMD;
	dlg->msg.spacing = QMS;

	mlk_message_start(&dlg->msg);
	mlk_message_query(&dlg->msg, NULL, &dlg->msg.h);

	dlg->action.data = dlg;
	dlg->action.handle = handle;
	dlg->action.update = update;
	dlg->action.draw = draw;
	dlg->action.end = end;

	return &dlg->action;
}
