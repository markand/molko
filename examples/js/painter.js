/*
 * painter.js -- simple example to use the painter
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

const w = new Molko.Window("Example - Painter", 1280, 720);
const p = new Molko.Painter();

for (var run = true; run;) {
	for (var ev; ev = Molko.Event.poll(); ) {
		switch (ev.type) {
		case Molko.Event.QUIT:
			run = false;
			break;
		default:
			break;
		}
	}

	/* Change a color. */
	p.color = 0x3c5e8bff;
	p.clear();

	/* Draw using individual arguments. */
	p.color = 0x7a367bff;
	p.drawLine(10, 10, 50, 10);
	p.drawRectangle(10, 20, 20, 20);
	p.drawCircle(20, 65, 10);

	/* Draw using objects. */
	p.color = 0x884b2bff;
	p.drawLine({x1: 10, y1: 100, x2: 50, y2: 100});
	p.drawRectangle({x: 10, y: 120, w: 20, h: 20});
	p.drawCircle({x: 20, y: 165, r: 10});

	p.present();
}
