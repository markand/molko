/*
 * key.h -- keyboard definitions
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

#ifndef MLK_CORE_KEY_H
#define MLK_CORE_KEY_H

/**
 * \file mlk/core/key.h
 * \brief Keyboard definitions
 */

/**
 * \enum mlk_key
 * \brief Keyboard code
 *
 * Its enumeration constants are as self explanatory as possible.
 */
enum mlk_key {
	/**
	 * Unknown key pressed (default).
	 */
	MLK_KEY_UNKNOWN = 0,

	/**
	 * Return/enter.
	 */
	MLK_KEY_ENTER,

	/**
	 * Escape, aka the vim key.
	 */
	MLK_KEY_ESCAPE,

	/**
	 * Left delete.
	 */
	MLK_KEY_BACKSPACE,

	/**
	 * Tab.
	 */
	MLK_KEY_TAB,

	/**
	 * Space bar.
	 */
	MLK_KEY_SPACE,

	/**
	 * Exclamation (!).
	 */
	MLK_KEY_EXCLAIM,

	/**
	 * Double quot (").
	 */
	MLK_KEY_DOUBLE_QUOTE,

	/**
	 * Hash (#).
	 */
	MLK_KEY_HASH,

	/**
	 * Percent (%).
	 */
	MLK_KEY_PERCENT,

	/**
	 * Euro ($).
	 */
	MLK_KEY_DOLLAR,

	/**
	 * Ampersand (&).
	 */
	MLK_KEY_AMPERSAND,

	/**
	 * Simple quote (').
	 */
	MLK_KEY_QUOTE,

	/**
	 * Left parenthesis (().
	 */
	MLK_KEY_LPAREN,

	/**
	 * Right parenthesis ()).
	 */
	MLK_KEY_RPAREN,

	/**
	 * Asterisk (*).
	 */
	MLK_KEY_ASTERISK,

	/**
	 * Plus (+).
	 */
	MLK_KEY_PLUS,

	/**
	 * Comma (,).
	 */
	MLK_KEY_COMMA,

	/**
	 * Minus (-).
	 */
	MLK_KEY_MINUS,

	/**
	 * Period (.).
	 */
	MLK_KEY_PERIOD,

	/**
	 * Forward slash (/).
	 */
	MLK_KEY_SLASH,

	/**
	 * 0.
	 */
	MLK_KEY_0,

	/**
	 * 1.
	 */
	MLK_KEY_1,

	/**
	 * 2.
	 */
	MLK_KEY_2,

	/**
	 * 3.
	 */
	MLK_KEY_3,

	/**
	 * 4.
	 */
	MLK_KEY_4,

	/**
	 * 5.
	 */
	MLK_KEY_5,

	/**
	 * 6.
	 */
	MLK_KEY_6,

	/**
	 * 7.
	 */
	MLK_KEY_7,

	/**
	 * 8.
	 */
	MLK_KEY_8,

	/**
	 * 9.
	 */
	MLK_KEY_9,

	/**
	 * Colon (:).
	 */
	MLK_KEY_COLON,

	/**
	 * Semicolon (;).
	 */
	MLK_KEY_SEMICOLON,

	/**
	 * Less (<).
	 */
	MLK_KEY_LESS,

	/**
	 * Equals (=).
	 */
	MLK_KEY_EQUALS,

	/**
	 * Greater (>).
	 */
	MLK_KEY_GREATER,

	/**
	 * Response (?).
	 */
	MLK_KEY_QUESTION,

	/**
	 * At (@).
	 */
	MLK_KEY_AT,

	/**
	 * Left bracket ([).
	 */
	MLK_KEY_LBRACKET,

	/**
	 * Back slack (\).
	 */
	MLK_KEY_BACKSLASH,

	/**
	 * Right bracket (]).
	 */
	MLK_KEY_RBRACKET,

	/**
	 * Caret (^).
	 */
	MLK_KEY_CARET,

	/**
	 * Underscore (_).
	 */
	MLK_KEY_UNDERSCORE,

	/**
	 * Back quote (`).
	 */
	MLK_KEY_BACKQUOTE,

	/**
	 * A.
	 */
	MLK_KEY_A,

	/**
	 * B.
	 */
	MLK_KEY_B,

	/**
	 * C.
	 */
	MLK_KEY_C,

	/**
	 * D.
	 */
	MLK_KEY_D,

	/**
	 * E.
	 */
	MLK_KEY_E,

	/**
	 * F.
	 */
	MLK_KEY_F,

	/**
	 * G.
	 */
	MLK_KEY_G,

	/**
	 * H.
	 */
	MLK_KEY_H,

	/**
	 * I.
	 */
	MLK_KEY_I,

	/**
	 * J.
	 */
	MLK_KEY_J,

	/**
	 * K.
	 */
	MLK_KEY_K,

	/**
	 * L.
	 */
	MLK_KEY_L,

	/**
	 * M.
	 */
	MLK_KEY_M,

	/**
	 * N.
	 */
	MLK_KEY_N,

	/**
	 * O.
	 */
	MLK_KEY_O,

	/**
	 * P.
	 */
	MLK_KEY_P,

	/**
	 * Q.
	 */
	MLK_KEY_Q,

	/**
	 * R.
	 */
	MLK_KEY_R,

	/**
	 * S.
	 */
	MLK_KEY_S,

	/**
	 * T.
	 */
	MLK_KEY_T,

	/**
	 * U.
	 */
	MLK_KEY_U,

	/**
	 * V.
	 */
	MLK_KEY_V,

	/**
	 * W.
	 */
	MLK_KEY_W,

	/**
	 * X.
	 */
	MLK_KEY_X,

	/**
	 * Y.
	 */
	MLK_KEY_Y,

	/**
	 * Z.
	 */
	MLK_KEY_Z,

	/**
	 * Caps lock, aka the most useless key.
	 */
	MLK_KEY_CAPSLOCK,

	/**
	 * F1.
	 */
	MLK_KEY_F1,

	/**
	 * F2.
	 */
	MLK_KEY_F2,

	/**
	 * F3.
	 */
	MLK_KEY_F3,

	/**
	 * F4.
	 */
	MLK_KEY_F4,

	/**
	 * F5.
	 */
	MLK_KEY_F5,

	/**
	 * F6.
	 */
	MLK_KEY_F6,

	/**
	 * F7.
	 */
	MLK_KEY_F7,

	/**
	 * F8.
	 */
	MLK_KEY_F8,

	/**
	 * F9.
	 */
	MLK_KEY_F9,

	/**
	 * F10.
	 */
	MLK_KEY_F10,

	/**
	 * F11.
	 */
	MLK_KEY_F11,

	/**
	 * F12.
	 */
	MLK_KEY_F12,

	/**
	 * F13.
	 */
	MLK_KEY_F13,

	/**
	 * F14.
	 */
	MLK_KEY_F14,

	/**
	 * F15.
	 */
	MLK_KEY_F15,

	/**
	 * F16.
	 */
	MLK_KEY_F16,

	/**
	 * F17.
	 */
	MLK_KEY_F17,

	/**
	 * F18.
	 */
	MLK_KEY_F18,

	/**
	 * F19.
	 */
	MLK_KEY_F19,

	/**
	 * F20.
	 */
	MLK_KEY_F20,

	/**
	 * F21.
	 */
	MLK_KEY_F21,

	/**
	 * F22.
	 */
	MLK_KEY_F22,

	/**
	 * F23.
	 */
	MLK_KEY_F23,

	/**
	 * F24.
	 */
	MLK_KEY_F24,

	/**
	 * Print screen.
	 */
	MLK_KEY_PRINTSCREEN,

	/**
	 * Scroll lock.
	 */
	MLK_KEY_SCROLL_LOCK,

	/**
	 * Pause.
	 */
	MLK_KEY_PAUSE,

	/**
	 * Insert.
	 */
	MLK_KEY_INSERT,

	/**
	 * Home.
	 */
	MLK_KEY_HOME,

	/**
	 * Page up.
	 */
	MLK_KEY_PAGEUP,

	/**
	 * Right delete.
	 */
	MLK_KEY_DELETE,

	/**
	 * End.
	 */
	MLK_KEY_END,

	/**
	 * Page down.
	 */
	MLK_KEY_PAGEDOWN,

	/**
	 * Right arrow.
	 */
	MLK_KEY_RIGHT,

	/**
	 * Left arrow.
	 */
	MLK_KEY_LEFT,

	/**
	 * Down arrow.
	 */
	MLK_KEY_DOWN,

	/**
	 * Up arrow.
	 */
	MLK_KEY_UP,

	/**
	 * Num lock key.
	 */
	MLK_KEY_NUMLOCKCLEAR,

	/**
	 * Keypad divide (/).
	 */
	MLK_KEY_KP_DIVIDE,

	/**
	 * Keypad multiply (*).
	 */
	MLK_KEY_KP_MULTIPLY,

	/**
	 * Keypad minus (-).
	 */
	MLK_KEY_KP_MINUS,

	/**
	 * Keypad plus (+).
	 */
	MLK_KEY_KP_PLUS,

	/**
	 * Keypad enter.
	 */
	MLK_KEY_KP_ENTER,

	/**
	 * Keypad 00.
	 */
	MLK_KEY_KP_00,

	/**
	 * Keypad 000.
	 */
	MLK_KEY_KP_000,

	/**
	 * Keypad (1).
	 */
	MLK_KEY_KP_1,

	/**
	 * Keypad (2).
	 */
	MLK_KEY_KP_2,

	/**
	 * Keypad (3).
	 */
	MLK_KEY_KP_3,

	/**
	 * Keypad (4).
	 */
	MLK_KEY_KP_4,

	/**
	 * Keypad (5).
	 */
	MLK_KEY_KP_5,

	/**
	 * Keypad (6).
	 */
	MLK_KEY_KP_6,

	/**
	 * Keypad (7).
	 */
	MLK_KEY_KP_7,

	/**
	 * Keypad (8).
	 */
	MLK_KEY_KP_8,

	/**
	 * Keypad (9).
	 */
	MLK_KEY_KP_9,

	/**
	 * Keypad (0).
	 */
	MLK_KEY_KP_0,

	/**
	 * Keypad period (.).
	 */
	MLK_KEY_KP_PERIOD,

	/**
	 * Keypad comma (,).
	 */
	MLK_KEY_KP_COMMA,

	/**
	 * Menu.
	 */
	MLK_KEY_MENU,

	/**
	 * Sound mute.
	 */
	MLK_KEY_MUTE,

	/**
	 * Sound volume up.
	 */
	MLK_KEY_VOLUME_UP,

	/**
	 * Sound volume down.
	 */
	MLK_KEY_VOLUME_DOWN,

	/**
	 * Left ctrl.
	 */
	MLK_KEY_LCTRL,

	/**
	 * Left shift.
	 */
	MLK_KEY_LSHIFT,

	/**
	 * Left alt/option.
	 */
	MLK_KEY_LALT,

	/**
	 * Left super (or logo).
	 */
	MLK_KEY_LSUPER,

	/**
	 * Right ctrl.
	 */
	MLK_KEY_RCTRL,

	/**
	 * Right shift.
	 */
	MLK_KEY_RSHIFT,

	/**
	 * Right alt/option.
	 */
	MLK_KEY_RALT,

	/**
	 * Right super (or logo).
	 */
	MLK_KEY_RSUPER,
};

/**
 * \enum mlk_key_mod
 * \brief Keyboard modifier
 *
 * This enumeration is implemented as a bitmask.
 */
enum mlk_key_mod {
	/**
	 * Left shift.
	 */
	MLK_KEY_MOD_LSHIFT      = 1 << 0,

	/**
	 * Left ctrl.
	 */
	MLK_KEY_MOD_LCTRL       = 1 << 1,

	/**
	 * Left alt/option.
	 */
	MLK_KEY_MOD_LALT        = 1 << 2,

	/**
	 * Left super (or logo).
	 */
	MLK_KEY_MOD_LSUPER      = 1 << 3,

	/**
	 * Right shift.
	 */
	MLK_KEY_MOD_RSHIFT      = 1 << 4,

	/**
	 * Right ctrl.
	 */
	MLK_KEY_MOD_RCTRL       = 1 << 5,

	/**
	 * Right alt/option.
	 */
	MLK_KEY_MOD_RALT        = 1 << 6,

	/**
	 * Right super (or logo).
	 */
	MLK_KEY_MOD_RSUPER      = 1 << 7
};

#endif /* !MLK_CORE_KEY_H */
