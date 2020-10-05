/*
 * rbuf.h -- basic utility for reading input buffers
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

#ifndef MOLKO_RBUF_H
#define MOLKO_RBUF_H

/**
 * \file rbuf.h
 * \brief Basic utility for reading input buffers.
 */

#include <stdbool.h>
#include <stddef.h>

/**
 * \brief Input object to store progression.
 */
struct rbuf {
	const char *s;  /*!< (RO) Pointer to current character. */
	const char *e;  /*!< (RO) Pointer to end of array. */
};

/**
 * Open this input buffer.
 *
 * \pre rb != NULL
 * \pre data != NULL
 * \param rb the input object
 * \param data the data to read
 * \param datasz the size of buffer
 */
void
rbuf_open(struct rbuf *rb, const void *data, size_t datasz);

/**
 * Read the next line from the input.
 *
 * This function writes at most outputsz - 1 and is always NULL terminated
 * unless the function returns false which indicates either end of buffer or
 * too small output size.
 *
 * Example of use (`some_data` is assumed to be a buffer).
 *
 * \code
 * struct rbuf rb;
 * char line[1024];
 *
 * rbuf_open(&rb, some_data, sizeof (some_data));
 *
 * while (rbuf_readline(&rb, line, sizeof (line))) {
 *     printf("line contents: >[%s]<\n", line);
 * }
 * \endcode
 *
 * \pre rb != NULL
 * \pre output != NULL
 * \pre outputsz > 0
 * \param rb the input object
 * \param output the output buffer
 * \param outputsz the output buffer size
 * \return true if a line was read correctly
 */
bool
rbuf_readline(struct rbuf *rb, char *output, size_t outputsz);

#endif /* !MOLKO_RBUF_H */
