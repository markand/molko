/*
 * event.c -- event management
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

#include <SDL.h>

#include "event.h"

/* Maintain with enum key constants in key.h */
static const struct {
	SDL_Keycode key;
	enum key value;
} keymap[] = {
	{ SDLK_RETURN,          KEY_ENTER               },
	{ SDLK_ESCAPE,          KEY_ESCAPE              },
	{ SDLK_BACKSPACE,       KEY_BACKSPACE           },
	{ SDLK_TAB,             KEY_TAB                 },
	{ SDLK_SPACE,           KEY_SPACE               },
	{ SDLK_EXCLAIM,         KEY_EXCLAIM             },
	{ SDLK_QUOTEDBL,        KEY_DOUBLE_QUOTE        },
	{ SDLK_HASH,            KEY_HASH                },
	{ SDLK_PERCENT,         KEY_PERCENT             },
	{ SDLK_DOLLAR,          KEY_DOLLAR              },
	{ SDLK_AMPERSAND,       KEY_AMPERSAND           },
	{ SDLK_QUOTE,           KEY_QUOTE               },
	{ SDLK_LEFTPAREN,       KEY_LEFT_PAREN          },
	{ SDLK_RIGHTPAREN,      KEY_RIGHT_PAREN         },
	{ SDLK_ASTERISK,        KEY_ASTERISK            },
	{ SDLK_PLUS,            KEY_PLUS                },
	{ SDLK_COMMA,           KEY_COMMA               },
	{ SDLK_MINUS,           KEY_MINUS               },
	{ SDLK_PERIOD,          KEY_PERIOD              },
	{ SDLK_SLASH,           KEY_SLASH               },
	{ SDLK_0,               KEY_0                   },
	{ SDLK_1,               KEY_1                   },
	{ SDLK_2,               KEY_2                   },
	{ SDLK_3,               KEY_3                   },
	{ SDLK_4,               KEY_4                   },
	{ SDLK_5,               KEY_5                   },
	{ SDLK_6,               KEY_6                   },
	{ SDLK_7,               KEY_7                   },
	{ SDLK_8,               KEY_8                   },
	{ SDLK_9,               KEY_9                   },
	{ SDLK_COLON,           KEY_COLON               },
	{ SDLK_SEMICOLON,       KEY_SEMICOLON           },
	{ SDLK_LESS,            KEY_LESS                },
	{ SDLK_EQUALS,          KEY_EQUALS              },
	{ SDLK_GREATER,         KEY_GREATER             },
	{ SDLK_QUESTION,        KEY_QUESTION            },
	{ SDLK_AT,              KEY_AT                  },
	{ SDLK_LEFTBRACKET,     KEY_LEFT_BRACKET        },
	{ SDLK_BACKSLASH,       KEY_BACKSLASH           },
	{ SDLK_RIGHTBRACKET,    KEY_RIGHT_BRACKET       },
	{ SDLK_CARET,           KEY_CARET               },
	{ SDLK_UNDERSCORE,      KEY_UNDERSCORE          },
	{ SDLK_BACKQUOTE,       KEY_BACKQUOTE           },
	{ SDLK_a,               KEY_a                   },
	{ SDLK_b,               KEY_b                   },
	{ SDLK_c,               KEY_c                   },
	{ SDLK_d,               KEY_d                   },
	{ SDLK_e,               KEY_e                   },
	{ SDLK_f,               KEY_f                   },
	{ SDLK_g,               KEY_g                   },
	{ SDLK_h,               KEY_h                   },
	{ SDLK_i,               KEY_i                   },
	{ SDLK_j,               KEY_j                   },
	{ SDLK_k,               KEY_k                   },
	{ SDLK_l,               KEY_l                   },
	{ SDLK_m,               KEY_m                   },
	{ SDLK_n,               KEY_n                   },
	{ SDLK_o,               KEY_o                   },
	{ SDLK_p,               KEY_p                   },
	{ SDLK_q,               KEY_q                   },
	{ SDLK_r,               KEY_r                   },
	{ SDLK_s,               KEY_s                   },
	{ SDLK_t,               KEY_t                   },
	{ SDLK_u,               KEY_u                   },
	{ SDLK_v,               KEY_v                   },
	{ SDLK_w,               KEY_w                   },
	{ SDLK_x,               KEY_x                   },
	{ SDLK_y,               KEY_y                   },
	{ SDLK_z,               KEY_z                   },
	{ SDLK_CAPSLOCK,        KEY_CAPSLOCK            },
	{ SDLK_F1,              KEY_F1                  },
	{ SDLK_F2,              KEY_F2                  },
	{ SDLK_F3,              KEY_F3                  },
	{ SDLK_F4,              KEY_F4                  },
	{ SDLK_F5,              KEY_F5                  },
	{ SDLK_F6,              KEY_F6                  },
	{ SDLK_F7,              KEY_F7                  },
	{ SDLK_F8,              KEY_F8                  },
	{ SDLK_F9,              KEY_F9                  },
	{ SDLK_F10,             KEY_F10                 },
	{ SDLK_F11,             KEY_F11                 },
	{ SDLK_F12,             KEY_F12                 },
	{ SDLK_F13,             KEY_F13                 },
	{ SDLK_F14,             KEY_F14                 },
	{ SDLK_F15,             KEY_F15                 },
	{ SDLK_F16,             KEY_F16                 },
	{ SDLK_F17,             KEY_F17                 },
	{ SDLK_F18,             KEY_F18                 },
	{ SDLK_F19,             KEY_F19                 },
	{ SDLK_F20,             KEY_F20                 },
	{ SDLK_F21,             KEY_F21                 },
	{ SDLK_F22,             KEY_F22                 },
	{ SDLK_F23,             KEY_F23                 },
	{ SDLK_F24,             KEY_F24                 },
	{ SDLK_PRINTSCREEN,     KEY_PRINTSCREEN         },
	{ SDLK_SCROLLLOCK,      KEY_SCROLL_LOCK         },
	{ SDLK_PAUSE,           KEY_PAUSE               },
	{ SDLK_INSERT,          KEY_INSERT              },
	{ SDLK_HOME,            KEY_HOME                },
	{ SDLK_PAGEUP,          KEY_PAGEUP              },
	{ SDLK_DELETE,          KEY_DELETE              },
	{ SDLK_END,             KEY_END                 },
	{ SDLK_PAGEDOWN,        KEY_PAGEDOWN            },
	{ SDLK_RIGHT,           KEY_RIGHT               },
	{ SDLK_LEFT,            KEY_LEFT                },
	{ SDLK_DOWN,            KEY_DOWN                },
	{ SDLK_UP,              KEY_UP                  },
	{ SDLK_KP_DIVIDE,       KEY_KP_DIVIDE           },
	{ SDLK_KP_MULTIPLY,     KEY_KP_MULTIPLY         },
	{ SDLK_KP_MINUS,        KEY_KP_MINUS            },
	{ SDLK_KP_PLUS,         KEY_KP_PLUS             },
	{ SDLK_KP_ENTER,        KEY_KP_ENTER            },
	{ SDLK_KP_1,            KEY_KP_1                },
	{ SDLK_KP_2,            KEY_KP_2                },
	{ SDLK_KP_3,            KEY_KP_3                },
	{ SDLK_KP_4,            KEY_KP_4                },
	{ SDLK_KP_5,            KEY_KP_5                },
	{ SDLK_KP_6,            KEY_KP_6                },
	{ SDLK_KP_7,            KEY_KP_7                },
	{ SDLK_KP_8,            KEY_KP_8                },
	{ SDLK_KP_9,            KEY_KP_9                },
	{ SDLK_KP_0,            KEY_KP_0                },
	{ SDLK_KP_PERIOD,       KEY_KP_PERIOD           },
	{ SDLK_KP_COMMA,        KEY_KP_COMMA            },
	{ SDLK_MENU,            KEY_MENU                },
	{ SDLK_MUTE,            KEY_MUTE                },
	{ SDLK_VOLUMEUP,        KEY_VOLUME_UP           },
	{ SDLK_VOLUMEDOWN,      KEY_VOLUME_DOWN         },
	{ SDLK_LCTRL,           KEY_LCTRL               },
	{ SDLK_LSHIFT,          KEY_LSHIFT              },
	{ SDLK_LALT,            KEY_LALT                },
	{ SDLK_LGUI,            KEY_LSUPER              },
	{ SDLK_RCTRL,           KEY_RCTRL               },
	{ SDLK_RSHIFT,          KEY_RSHIFT              },
	{ SDLK_RALT,            KEY_RALT                },
	{ SDLK_RGUI,            KEY_RSUPER              },
	{ 0,                    -1                      }
};

/* Maintain with enum mouse_button constants in mouse.h */
static const struct {
	int key;
	enum mouse_button value;
} buttons[] = {
	{ SDL_BUTTON_LEFT,      MOUSE_BUTTON_LEFT       },
	{ SDL_BUTTON_MIDDLE,    MOUSE_BUTTON_MIDDLE     },
	{ SDL_BUTTON_RIGHT,     MOUSE_BUTTON_RIGHT      },
	{ -1,                   MOUSE_BUTTON_NONE       }
};

static void
convert_key(const SDL_Event *event, union event *ev)
{
	ev->type = event->type == SDL_KEYDOWN ? EVENT_KEYDOWN : EVENT_KEYUP;
	ev->key.key = KEY_UNKNOWN;

	for (size_t i = 0; keymap[i].key != 0; ++i) {
		if (keymap[i].key == event->key.keysym.sym) {
			ev->key.key = keymap[i].value;
			break;
		}
	}
}

static void
convert_mouse(const SDL_Event *event, union event *ev)
{
	ev->type = EVENT_MOUSE;
	ev->mouse.buttons = 0;
	ev->mouse.x = event->motion.x;
	ev->mouse.y = event->motion.y;

	if (event->motion.state & SDL_BUTTON_LMASK)
		ev->mouse.buttons |= MOUSE_BUTTON_LEFT;
	if (event->motion.state & SDL_BUTTON_MMASK)
		ev->mouse.buttons |= MOUSE_BUTTON_MIDDLE;
	if (event->motion.state & SDL_BUTTON_RMASK)
		ev->mouse.buttons |= MOUSE_BUTTON_RIGHT;
}

static void
convert_click(const SDL_Event *event, union event *ev)
{
	ev->type = event->type == SDL_MOUSEBUTTONDOWN ? EVENT_CLICKDOWN : EVENT_CLICKUP;
	ev->click.button = MOUSE_BUTTON_NONE;
	ev->click.x = event->button.x;
	ev->click.y = event->button.y;

	for (size_t i = 0; buttons[i].value != MOUSE_BUTTON_NONE; ++i) {
		if (buttons[i].key == event->button.button) {
			ev->click.button = buttons[i].value;
			break;
		}
	}
}

bool
event_poll(union event *ev)
{
	SDL_Event event;

	/*
	 * Loop until we find an event we want to report, we skip unneeded
	 * ones.
	 */
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			ev->type = EVENT_QUIT;
			return true;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			convert_key(&event, ev);
			return true;
		case SDL_MOUSEMOTION:
			convert_mouse(&event, ev);
			return true;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			convert_click(&event, ev);
			return true;
		default:
			continue;
		}
	}

	return false;
}
