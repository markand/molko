/*
 * ui_p.h -- private helpers for libmlk-ui
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

#ifndef MLK_UI_P_H
#define MLK_UI_P_H

#include "sysconfig.h"

#define MLK__STYLE_CALL(s, def, f, ...) \
        ((s) && (s)->f) ? (s)->f((s), __VA_ARGS__) : (def)->f ? (def)->f((s) ? (s) : (def), __VA_ARGS__) : 0

#define MLK__STYLE_FONT(f, d) \
        f ? f : &mlk_ui_fonts[d]

// TODO: DEPRECATED
#define MLK__DELEGATE_INVOKE(d, def, f, ...)            \
do {                                                    \
        if (d && d->f)                                  \
                d->f(d, __VA_ARGS__);                   \
        else if (def.f)                                 \
                def.f(&def, __VA_ARGS__);               \
} while (0)

// TODO: DEPRECATED
#define MLK__DELEGATE_INVOKE_RET(d, def, f, ...)        \
do {                                                    \
        if (d && d->f)                                  \
                return d->f(d, __VA_ARGS__);            \
        if (def.f)                                      \
                return def.f(&def, __VA_ARGS__);        \
} while (0)

#define MLK__STYLE(w, d)                                \
        (w->style ? w->style : d)

#if defined(MLK_WITH_NLS)
#       include <libintl.h>
#       define _(s) dgettext("libmlk-ui", s)
#else
#       define _(s) s
#endif

#endif /* !MLK_UI_P_H */
