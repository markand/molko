/*
 * key.h -- keyboard definitions
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

#ifndef MOLKO_KEY_H
#define MOLKO_KEY_H

/**
 * \file key.h
 * \brief Keyboard definitions.
 * \ingroup input
 */

/**
 * \brief Key codes.
 */
enum key {
	KEY_UNKNOWN,
	KEY_ENTER,
	KEY_ESCAPE,
	KEY_BACKSPACE,
	KEY_TAB,
	KEY_SPACE,
	KEY_EXCLAIM,
	KEY_DOUBLE_QUOTE,
	KEY_HASH,
	KEY_PERCENT,
	KEY_DOLLAR,
	KEY_AMPERSAND,
	KEY_QUOTE,
	KEY_LEFT_PAREN,
	KEY_RIGHT_PAREN,
	KEY_ASTERISK,
	KEY_PLUS,
	KEY_COMMA,
	KEY_MINUS,
	KEY_PERIOD,
	KEY_SLASH,
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_COLON,
	KEY_SEMICOLON,
	KEY_LESS,
	KEY_EQUALS,
	KEY_GREATER,
	KEY_QUESTION,
	KEY_AT,
	KEY_LEFT_BRACKET,
	KEY_BACKSLASH,
	KEY_RIGHT_BRACKET,
	KEY_CARET,
	KEY_UNDERSCORE,
	KEY_BACKQUOTE,
	KEY_a,
	KEY_b,
	KEY_c,
	KEY_d,
	KEY_e,
	KEY_f,
	KEY_g,
	KEY_h,
	KEY_i,
	KEY_j,
	KEY_k,
	KEY_l,
	KEY_m,
	KEY_n,
	KEY_o,
	KEY_p,
	KEY_q,
	KEY_r,
	KEY_s,
	KEY_t,
	KEY_u,
	KEY_v,
	KEY_w,
	KEY_x,
	KEY_y,
	KEY_z,
	KEY_CAPSLOCK,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_F13,
	KEY_F14,
	KEY_F15,
	KEY_F16,
	KEY_F17,
	KEY_F18,
	KEY_F19,
	KEY_F20,
	KEY_F21,
	KEY_F22,
	KEY_F23,
	KEY_F24,
	KEY_PRINTSCREEN,
	KEY_SCROLLLOCK,
	KEY_PAUSE,
	KEY_INSERT,
	KEY_HOME,
	KEY_PAGEUP,
	KEY_DELETE,
	KEY_END,
	KEY_PAGEDOWN,
	KEY_RIGHT,
	KEY_LEFT,
	KEY_DOWN,
	KEY_UP,
	KEY_NUMLOCKCLEAR,
	KEY_KP_DIVIDE,
	KEY_KP_MULTIPLY,
	KEY_KP_MINUS,
	KEY_KP_PLUS,
	KEY_KP_ENTER,
	KEY_KP_00,
	KEY_KP_000,
	KEY_KP_1,
	KEY_KP_2,
	KEY_KP_3,
	KEY_KP_4,
	KEY_KP_5,
	KEY_KP_6,
	KEY_KP_7,
	KEY_KP_8,
	KEY_KP_9,
	KEY_KP_0,
	KEY_KP_PERIOD,
	KEY_KP_COMMA,
	KEY_MENU,
	KEY_MUTE,
	KEY_VOLUME_UP,
	KEY_VOLUME_DOWN,
	KEY_LCTRL,
	KEY_LSHIFT,
	KEY_LALT,
	KEY_LSUPER,
	KEY_RCTRL,
	KEY_RSHIFT,
	KEY_RALT,
	KEY_RSUPER,
};

/**
 * \brief Keybord modifiers.
 *
 * This enumeration is usually stored as OR'ed flags as several modifiers can
 * be pressed at a time.
 */
enum keymod {
	KEYMOD_LSHIFT   = 1 << 0,       /*!< Left shift */
	KEYMOD_LCTRL    = 1 << 1,       /*!< Left control */
	KEYMOD_LALT     = 1 << 2,       /*!< Left alt */
	KEYMOD_LSUPER   = 1 << 3,       /*!< Left super (logo) */
	KEYMOD_RSHIFT   = 1 << 4,       /*!< Right shift */
	KEYMOD_RCTRL    = 1 << 5,       /*!< Right control */
	KEYMOD_RALT     = 1 << 6,       /*!< Right alt */
	KEYMOD_RSUPER   = 1 << 7        /*!< Right super (logo) */
};

#endif /* !MOLKO_KEY_H */
