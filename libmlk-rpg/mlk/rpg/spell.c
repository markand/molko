/*
 * spell.c -- magic spells
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

#include "spell.h"

void
spell_select(const struct spell *s, const struct battle *bt, struct selection *slt)
{
	assert(s && s->select);
	assert(bt);
	assert(slt);

	s->select(bt, slt);
}

void
spell_action(const struct spell *s, struct battle *bt, struct character *owner, const struct selection *slt)
{
	assert(s && s->action);
	assert(bt);
	assert(owner);
	assert(slt);

	s->action(bt, owner, slt);
}

void
spell_use(struct spell *s, struct character *owner, const struct selection *slt)
{
	assert(s && s->use);
	assert(owner);
	assert(slt);

	s->use(owner, slt);
}
