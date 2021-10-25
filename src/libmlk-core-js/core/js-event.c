/*
 * js-event.c -- core event binding
 *
 * Copyright (c) 2020-2021 David Demelier <markand@malikania.fr>
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

#include <core/alloc.h>
#include <core/event.h>

#include "js-event.h"

#define SIGNATURE DUK_HIDDEN_SYMBOL("Mlk.Event")

static void
push_click(duk_context *ctx, const union event *ev)
{
	duk_push_int(ctx, ev->click.button);
	duk_put_prop_string(ctx, -2, "button");
	duk_push_int(ctx, ev->click.x);
	duk_put_prop_string(ctx, -2, "x");
	duk_push_int(ctx, ev->click.y);
	duk_put_prop_string(ctx, -2, "y");
	duk_push_int(ctx, ev->click.clicks);
	duk_put_prop_string(ctx, -2, "clicks");
}

static void
push_key(duk_context *ctx, const union event *ev)
{
	duk_push_int(ctx, ev->key.key);
	duk_put_prop_string(ctx, -2, "key");
}

static void
push_mouse(duk_context *ctx, const union event *ev)
{
	duk_push_int(ctx, ev->mouse.buttons);
	duk_put_prop_string(ctx, -2, "buttons");
	duk_push_int(ctx, ev->mouse.x);
	duk_put_prop_string(ctx, -2, "x");
	duk_push_int(ctx, ev->mouse.y);
	duk_put_prop_string(ctx, -2, "y");
}

static void
push_quit(duk_context *ctx, const union event *ev)
{
	(void)ctx;
	(void)ev;
}

static void (*push[])(duk_context *, const union event *) = {
	[EVENT_CLICKDOWN] = push_click,
	[EVENT_CLICKUP] = push_click,
	[EVENT_KEYDOWN] = push_key,
	[EVENT_KEYUP] = push_key,
	[EVENT_MOUSE] = push_mouse,
	[EVENT_QUIT] = push_quit,
};

static duk_ret_t
Event_poll(duk_context *ctx)
{
	(void)ctx;

	union event ev;

	if (!event_poll(&ev))
		duk_push_null(ctx);
	else
		js_event_push(ctx, &ev);

	return 1;
}

static const duk_function_list_entry functions[] = {
	{ "poll",       Event_poll,     0               },
	{ NULL,         NULL,           0               }
};

static const duk_number_list_entry types[] = {
	{ "CLICKDOWN",          EVENT_CLICKDOWN         },
	{ "CLICKUP",            EVENT_CLICKUP           },
	{ "KEYDOWN",            EVENT_KEYDOWN           },
	{ "KEYUP",              EVENT_KEYUP             },
	{ "MOUSE",              EVENT_MOUSE             },
	{ "QUIT",               EVENT_QUIT              },
	{ NULL,                 0                       }
};

static const duk_number_list_entry keys[] = {
	{ "UNKNOWN",            KEY_UNKNOWN             },
	{ "ENTER",              KEY_ENTER               },
	{ "ESCAPE",             KEY_ESCAPE              },
	{ "BACKSPACE",          KEY_BACKSPACE           },
	{ "TAB",                KEY_TAB                 },
	{ "SPACE",              KEY_SPACE               },
	{ "EXCLAIM",            KEY_EXCLAIM             },
	{ "DOUBLE_QUOTE",       KEY_DOUBLE_QUOTE        },
	{ "HASH",               KEY_HASH                },
	{ "PERCENT",            KEY_PERCENT             },
	{ "DOLLAR",             KEY_DOLLAR              },
	{ "AMPERSAND",          KEY_AMPERSAND           },
	{ "QUOTE",              KEY_QUOTE               },
	{ "LEFT_PAREN",         KEY_LEFT_PAREN          },
	{ "RIGHT_PAREN",        KEY_RIGHT_PAREN         },
	{ "ASTERISK",           KEY_ASTERISK            },
	{ "PLUS",               KEY_PLUS                },
	{ "COMMA",              KEY_COMMA               },
	{ "MINUS",              KEY_MINUS               },
	{ "PERIOD",             KEY_PERIOD              },
	{ "SLASH",              KEY_SLASH               },
	{ "0",                  KEY_0                   },
	{ "1",                  KEY_1                   },
	{ "2",                  KEY_2                   },
	{ "3",                  KEY_3                   },
	{ "4",                  KEY_4                   },
	{ "5",                  KEY_5                   },
	{ "6",                  KEY_6                   },
	{ "7",                  KEY_7                   },
	{ "8",                  KEY_8                   },
	{ "9",                  KEY_9                   },
	{ "COLON",              KEY_COLON               },
	{ "SEMICOLON",          KEY_SEMICOLON           },
	{ "LESS",               KEY_LESS                },
	{ "EQUALS",             KEY_EQUALS              },
	{ "GREATER",            KEY_GREATER             },
	{ "QUESTION",           KEY_QUESTION            },
	{ "AT",                 KEY_AT                  },
	{ "LEFT_BRACKET",       KEY_LEFT_BRACKET        },
	{ "BACKSLASH",          KEY_BACKSLASH           },
	{ "RIGHT_BRACKET",      KEY_RIGHT_BRACKET       },
	{ "CARET",              KEY_CARET               },
	{ "UNDERSCORE",         KEY_UNDERSCORE          },
	{ "BACKQUOTE",          KEY_BACKQUOTE           },
	{ "a",                  KEY_a                   },
	{ "b",                  KEY_b                   },
	{ "c",                  KEY_c                   },
	{ "d",                  KEY_d                   },
	{ "e",                  KEY_e                   },
	{ "f",                  KEY_f                   },
	{ "g",                  KEY_g                   },
	{ "h",                  KEY_h                   },
	{ "i",                  KEY_i                   },
	{ "j",                  KEY_j                   },
	{ "k",                  KEY_k                   },
	{ "l",                  KEY_l                   },
	{ "m",                  KEY_m                   },
	{ "n",                  KEY_n                   },
	{ "o",                  KEY_o                   },
	{ "p",                  KEY_p                   },
	{ "q",                  KEY_q                   },
	{ "r",                  KEY_r                   },
	{ "s",                  KEY_s                   },
	{ "t",                  KEY_t                   },
	{ "u",                  KEY_u                   },
	{ "v",                  KEY_v                   },
	{ "w",                  KEY_w                   },
	{ "x",                  KEY_x                   },
	{ "y",                  KEY_y                   },
	{ "z",                  KEY_z                   },
	{ "CAPSLOCK",           KEY_CAPSLOCK            },
	{ "F1",                 KEY_F1                  },
	{ "F2",                 KEY_F2                  },
	{ "F3",                 KEY_F3                  },
	{ "F4",                 KEY_F4                  },
	{ "F5",                 KEY_F5                  },
	{ "F6",                 KEY_F6                  },
	{ "F7",                 KEY_F7                  },
	{ "F8",                 KEY_F8                  },
	{ "F9",                 KEY_F9                  },
	{ "F10",                KEY_F10                 },
	{ "F11",                KEY_F11                 },
	{ "F12",                KEY_F12                 },
	{ "F13",                KEY_F13                 },
	{ "F14",                KEY_F14                 },
	{ "F15",                KEY_F15                 },
	{ "F16",                KEY_F16                 },
	{ "F17",                KEY_F17                 },
	{ "F18",                KEY_F18                 },
	{ "F19",                KEY_F19                 },
	{ "F20",                KEY_F20                 },
	{ "F21",                KEY_F21                 },
	{ "F22",                KEY_F22                 },
	{ "F23",                KEY_F23                 },
	{ "F24",                KEY_F24                 },
	{ "PRINTSCREEN",        KEY_PRINTSCREEN         },
	{ "SCROLL_LOCK",        KEY_SCROLL_LOCK         },
	{ "PAUSE",              KEY_PAUSE               },
	{ "INSERT",             KEY_INSERT              },
	{ "HOME",               KEY_HOME                },
	{ "PAGEUP",             KEY_PAGEUP              },
	{ "DELETE",             KEY_DELETE              },
	{ "END",                KEY_END                 },
	{ "PAGEDOWN",           KEY_PAGEDOWN            },
	{ "RIGHT",              KEY_RIGHT               },
	{ "LEFT",               KEY_LEFT                },
	{ "DOWN",               KEY_DOWN                },
	{ "UP",                 KEY_UP                  },
	{ "NUMLOCKCLEAR",       KEY_NUMLOCKCLEAR        },
	{ "KP_DIVIDE",          KEY_KP_DIVIDE           },
	{ "KP_MULTIPLY",        KEY_KP_MULTIPLY         },
	{ "KP_MINUS",           KEY_KP_MINUS            },
	{ "KP_PLUS",            KEY_KP_PLUS             },
	{ "KP_ENTER",           KEY_KP_ENTER            },
	{ "KP_00",              KEY_KP_00               },
	{ "KP_000",             KEY_KP_000              },
	{ "KP_1",               KEY_KP_1                },
	{ "KP_2",               KEY_KP_2                },
	{ "KP_3",               KEY_KP_3                },
	{ "KP_4",               KEY_KP_4                },
	{ "KP_5",               KEY_KP_5                },
	{ "KP_6",               KEY_KP_6                },
	{ "KP_7",               KEY_KP_7                },
	{ "KP_8",               KEY_KP_8                },
	{ "KP_9",               KEY_KP_9                },
	{ "KP_0",               KEY_KP_0                },
	{ "KP_PERIOD",          KEY_KP_PERIOD           },
	{ "KP_COMMA",           KEY_KP_COMMA            },
	{ "MENU",               KEY_MENU                },
	{ "MUTE",               KEY_MUTE                },
	{ "VOLUME_UP",          KEY_VOLUME_UP           },
	{ "VOLUME_DOWN",        KEY_VOLUME_DOWN         },
	{ "LCTRL",              KEY_LCTRL               },
	{ "LSHIFT",             KEY_LSHIFT              },
	{ "LALT",               KEY_LALT                },
	{ "LSUPER",             KEY_LSUPER              },
	{ "RCTRL",              KEY_RCTRL               },
	{ "RSHIFT",             KEY_RSHIFT              },
	{ "RALT",               KEY_RALT                },
	{ "RSUPER",             KEY_RSUPER              },
	{ NULL,                 0                       }
};

static const duk_number_list_entry keymods[] = {
	{ "LSHIFT",             KEYMOD_LSHIFT           },
	{ "LCTRL",              KEYMOD_LCTRL            },
	{ "LALT",               KEYMOD_LALT             },
	{ "LSUPER",             KEYMOD_LSUPER           },
	{ "RSHIFT",             KEYMOD_RSHIFT           },
	{ "RTCRL",              KEYMOD_RCTRL            },
	{ "RALT",               KEYMOD_RALT             },
	{ "RSUPER",             KEYMOD_RSUPER           },
	{ NULL,                 0                       }
};

static const duk_number_list_entry buttons[] = {
	{ "LEFT",               MOUSE_BUTTON_LEFT       },
	{ "MIDDLE",             MOUSE_BUTTON_MIDDLE     },
	{ "RIGHT",              MOUSE_BUTTON_RIGHT      },
	{ NULL,                 0                       }
};

static duk_ret_t
Event_destructor(duk_context *ctx)
{
	duk_get_prop_string(ctx, 0, SIGNATURE);
	free(duk_to_pointer(ctx, -1));
	duk_del_prop_string(ctx, 0, SIGNATURE);
	duk_pop(ctx);

	return 0;
}

void
js_event_bind(duk_context *ctx)
{
	assert(ctx);

	duk_push_global_object(ctx);
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, functions);
	duk_push_object(ctx);
	duk_put_number_list(ctx, -1, types);
	duk_put_prop_string(ctx, -2, "Type");
	duk_push_object(ctx);
	duk_put_number_list(ctx, -1, keys);
	duk_put_prop_string(ctx, -2, "key");
	duk_push_object(ctx);
	duk_put_number_list(ctx, -1, keymods);
	duk_put_prop_string(ctx, -2, "keymod");
	duk_push_object(ctx);
	duk_put_number_list(ctx, -1, buttons);
	duk_put_prop_string(ctx, -2, "button");
	duk_put_prop_string(ctx, -2, "Event");
	duk_pop(ctx);
}

void
js_event_push(duk_context *ctx, const union event *ev)
{
	assert(ctx);

	duk_push_object(ctx);
	duk_push_c_function(ctx, Event_destructor, 1);
	duk_set_finalizer(ctx, -2);
	duk_push_pointer(ctx, alloc_dup(ev, sizeof (*ev)));
	duk_put_prop_string(ctx, -2, SIGNATURE);
	duk_push_int(ctx, ev->type);
	duk_put_prop_string(ctx, -2, "type");
	push[ev->type](ctx, ev);
}

union event *
js_event_require(duk_context *ctx, duk_idx_t idx)
{
	assert(ctx);

	union event *ev = NULL;

	if (duk_is_object(ctx, idx)) {
		duk_get_prop_string(ctx, idx, SIGNATURE);
		ev = duk_to_pointer(ctx, -1);
		duk_pop(ctx);
	}

	if (!ev)
		return (void)duk_error(ctx, DUK_ERR_TYPE_ERROR, "not an Event object"), NULL;

	return ev;
}
