/*
 * plat.h -- non-portable platform specific code
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

#ifndef MOLKO_PLAT_H
#define MOLKO_PLAT_H

/**
 * \file plat.h
 * \brief Non-portable platform specific code.
 */

/*
 * This block is used for doxygen documentation, the macros here are never
 * exposed.
 */
#if defined(DOXYGEN)

/**
 * Printf specifier for function supporting the printf(3) syntax. This is
 * currently only supported on GCC/Clang
 */
#define PLAT_PRINTF(p1, p2)

#else

#if defined(__GNUC__)
#define PLAT_PRINTF(p1, p2) __attribute__ ((format (printf, p1, p2)))
#else
#define PLAT_PRINTF(p1, p2)
#endif

#endif /* !DOXYGEN  */

#endif /* !MOLKO_PLAT_H */
