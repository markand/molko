--
-- init.sql -- initialize database
--
-- Copyright (c) 2020 David Demelier <markand@malikania.fr>
--
-- Permission to use, copy, modify, and/or distribute this software for any
-- purpose with or without fee is hereby granted, provided that the above
-- copyright notice and this permission notice appear in all copies.
--
-- THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
-- WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
-- MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
-- ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
-- WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
-- ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
-- OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
--

BEGIN EXCLUSIVE TRANSACTION;

CREATE TABLE IF NOT EXISTS property(
	id      INTEGER PRIMARY KEY AUTOINCREMENT,
	key     TEXT NOT NULL UNIQUE,
	value   TEXT NOT NULL
);

INSERT OR IGNORE INTO property(key, value) VALUES ('molko.create-date', strftime('%s','now'));
INSERT OR IGNORE INTO property(key, value) VALUES ('molko.update-date', strftime('%s','now'));

COMMIT;
