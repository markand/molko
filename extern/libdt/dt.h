/*
 * dt.h -- minimalist C testing library
 *
 * Copyright (c) 2022 David Demelier <markand@malikania.fr>
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

#ifndef DT_H
#define DT_H

#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static size_t dt_ntests;
static size_t dt_nchecks;
static size_t dt_nfailures;

#ifndef DT_DOUBLE_TOLERANCE
#define DT_DOUBLE_TOLERANCE 0.001
#endif

#define DT_ASSERT(x)                                                                    \
do {                                                                                    \
        ++ dt_nchecks;                                                                  \
                                                                                        \
        if (!(x)) {                                                                     \
                ++ dt_nfailures;                                                        \
                                                                                        \
                printf("%s:%d: assertion '" #x "' failed\n", __FILE__, __LINE__);       \
        }                                                                               \
} while (0)

#define DT_EQ(x, y, type, f)                                                            \
do {                                                                                    \
        const type tx = x;                                                              \
        const type ty = y;                                                              \
                                                                                        \
        ++ dt_nchecks;                                                                  \
                                                                                        \
        if (tx != ty) {                                                                 \
                ++ dt_nfailures;                                                        \
                                                                                        \
                printf("%s:%d: assertion " #x " == " #y " failed (" f " != " f ")\n",   \
                    __FILE__, __LINE__, tx, ty);                                        \
        }                                                                               \
} while (0)

#define DT_EQ_PTR(x, y)                                                                 \
do {                                                                                    \
        const void *tx = x;                                                             \
        const void *ty = y;                                                             \
                                                                                        \
        ++ dt_nchecks;                                                                  \
                                                                                        \
        if (tx != ty) {                                                                 \
                ++ dt_nfailures;                                                        \
                                                                                        \
                printf("%s:%d: assertion " #x " == " #y " failed (%p != %p)\n",         \
                    __FILE__, __LINE__, tx, ty);                                        \
        }                                                                               \
} while (0)

#define DT_EQ_STR(x, y)                                                                 \
do {                                                                                    \
        ++ dt_nchecks;                                                                  \
                                                                                        \
        if (strcmp(x, y) != 0) {                                                        \
                ++ dt_nfailures;                                                        \
                                                                                        \
                printf("%s:%d: assertion " #x " == " #y " failed (%s != %s)\n",         \
                    __FILE__, __LINE__, (x), (y));                                      \
        }                                                                               \
} while (0)

#define DT_EQ_DOUBLE(x, y)                                                              \
do {                                                                                    \
        const double tx = x;                                                            \
        const double ty = y;                                                            \
                                                                                        \
        ++ dt_nchecks;                                                                  \
                                                                                        \
        if (fabs(tx - ty) > DT_DOUBLE_TOLERANCE) {                                      \
                ++ dt_nfailures;                                                        \
                                                                                        \
                printf("%s:%d: assertion " #x " == " #y " failed (%f != %f)\n",         \
                    __FILE__, __LINE__, tx, ty);                                        \
        }                                                                               \
} while (0)

#define DT_RUN(f, ...)                                                                  \
do {                                                                                    \
        const size_t nchecks = dt_nchecks;                                              \
        const size_t nfailures = dt_nfailures;                                          \
                                                                                        \
        ++ dt_ntests;                                                                   \
                                                                                        \
        printf("== test " #f " ==\n");                                                  \
        f(__VA_ARGS__);                                                                 \
                                                                                        \
        printf("\n%zu checks, %zu failures\n\n",                                        \
            dt_nchecks - nchecks, dt_nfailures - nfailures);                            \
} while (0)

#define DT_SUMMARY()                                                                    \
do {                                                                                    \
        printf("summary: %zu tests, %zu checks, %zu failures\n",                        \
            dt_ntests, dt_nchecks, dt_nfailures);                                       \
} while (0)

/* Aliases for basic types. */
#define DT_EQ_CHAR(x, y)        DT_EQ(x, y, char, "%c")
#define DT_EQ_SHORT(x, y)       DT_EQ(x, y, short, "%hd")
#define DT_EQ_USHORT(x, y)      DT_EQ(x, y, unsigned short, "%hu")
#define DT_EQ_INT(x, y)         DT_EQ(x, y, int, "%d")
#define DT_EQ_UINT(x, y)        DT_EQ(x, y, unsigned int, "%u")
#define DT_EQ_LONG(x, y)        DT_EQ(x, y, long, "%ld")
#define DT_EQ_ULONG(x, y)       DT_EQ(x, y, unsigned long, "%lu")
#define DT_EQ_LLONG(x, y)       DT_EQ(x, y, long long, "%lld")
#define DT_EQ_ULLONG(x, y)      DT_EQ(x, y, unsigned long long, "%llu")
#define DT_EQ_INTMAX(x, y)      DT_EQ(x, y, intmax_t, "%jd")
#define DT_EQ_UINTMAX(x, y)     DT_EQ(x, y, uintmax_t, "%ju")

/* Aliases for fixed size integers. */
#define DT_EQ_INT8(x, y)        DT_EQ(x, y, int8_t, PRId8)
#define DT_EQ_UINT8(x, y)       DT_EQ(x, y, uint8_t int, PRIu8)
#define DT_EQ_INT16(x, y)       DT_EQ(x, y, int16_t, PRId16)
#define DT_EQ_UINT16(x, y)      DT_EQ(x, y, uint16_t int, PRIu16)
#define DT_EQ_INT32(x, y)       DT_EQ(x, y, int32_t, PRId32)
#define DT_EQ_UINT32(x, y)      DT_EQ(x, y, uint32_t int, PRIu32)
#define DT_EQ_INT64(x, y)       DT_EQ(x, y, int64_t, PRId64)
#define DT_EQ_UINT64(x, y)      DT_EQ(x, y, uint64_t int, PRIu64)

#endif /* !DT_H */
