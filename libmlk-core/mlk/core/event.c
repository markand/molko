/*
 * event.c -- event management
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

#include <SDL.h>

#include "event.h"

/* Maintain with enum key constants in key.h */
static const struct {
	SDL_Keycode key;
	enum mlk_key value;
} keymap[] = {
	{ SDLK_RETURN,          MLK_KEY_ENTER                   },
	{ SDLK_ESCAPE,          MLK_KEY_ESCAPE                  },
	{ SDLK_BACKSPACE,       MLK_KEY_BACKSPACE               },
	{ SDLK_TAB,             MLK_KEY_TAB                     },
	{ SDLK_SPACE,           MLK_KEY_SPACE                   },
	{ SDLK_EXCLAIM,         MLK_KEY_EXCLAIM                 },
	{ SDLK_QUOTEDBL,        MLK_KEY_DOUBLE_QUOTE            },
	{ SDLK_HASH,            MLK_KEY_HASH                    },
	{ SDLK_PERCENT,         MLK_KEY_PERCENT                 },
	{ SDLK_DOLLAR,          MLK_KEY_DOLLAR                  },
	{ SDLK_AMPERSAND,       MLK_KEY_AMPERSAND               },
	{ SDLK_QUOTE,           MLK_KEY_QUOTE                   },
	{ SDLK_LEFTPAREN,       MLK_KEY_LEFT_PAREN              },
	{ SDLK_RIGHTPAREN,      MLK_KEY_RIGHT_PAREN             },
	{ SDLK_ASTERISK,        MLK_KEY_ASTERISK                },
	{ SDLK_PLUS,            MLK_KEY_PLUS                    },
	{ SDLK_COMMA,           MLK_KEY_COMMA                   },
	{ SDLK_MINUS,           MLK_KEY_MINUS                   },
	{ SDLK_PERIOD,          MLK_KEY_PERIOD                  },
	{ SDLK_SLASH,           MLK_KEY_SLASH                   },
	{ SDLK_0,               MLK_KEY_0                       },
	{ SDLK_1,               MLK_KEY_1                       },
	{ SDLK_2,               MLK_KEY_2                       },
	{ SDLK_3,               MLK_KEY_3                       },
	{ SDLK_4,               MLK_KEY_4                       },
	{ SDLK_5,               MLK_KEY_5                       },
	{ SDLK_6,               MLK_KEY_6                       },
	{ SDLK_7,               MLK_KEY_7                       },
	{ SDLK_8,               MLK_KEY_8                       },
	{ SDLK_9,               MLK_KEY_9                       },
	{ SDLK_COLON,           MLK_KEY_COLON                   },
	{ SDLK_SEMICOLON,       MLK_KEY_SEMICOLON               },
	{ SDLK_LESS,            MLK_KEY_LESS                    },
	{ SDLK_EQUALS,          MLK_KEY_EQUALS                  },
	{ SDLK_GREATER,         MLK_KEY_GREATER                 },
	{ SDLK_QUESTION,        MLK_KEY_QUESTION                },
	{ SDLK_AT,              MLK_KEY_AT                      },
	{ SDLK_LEFTBRACKET,     MLK_KEY_LEFT_BRACKET            },
	{ SDLK_BACKSLASH,       MLK_KEY_BACKSLASH               },
	{ SDLK_RIGHTBRACKET,    MLK_KEY_RIGHT_BRACKET           },
	{ SDLK_CARET,           MLK_KEY_CARET                   },
	{ SDLK_UNDERSCORE,      MLK_KEY_UNDERSCORE              },
	{ SDLK_BACKQUOTE,       MLK_KEY_BACKQUOTE               },
	{ SDLK_a,               MLK_KEY_a                       },
	{ SDLK_b,               MLK_KEY_b                       },
	{ SDLK_c,               MLK_KEY_c                       },
	{ SDLK_d,               MLK_KEY_d                       },
	{ SDLK_e,               MLK_KEY_e                       },
	{ SDLK_f,               MLK_KEY_f                       },
	{ SDLK_g,               MLK_KEY_g                       },
	{ SDLK_h,               MLK_KEY_h                       },
	{ SDLK_i,               MLK_KEY_i                       },
	{ SDLK_j,               MLK_KEY_j                       },
	{ SDLK_k,               MLK_KEY_k                       },
	{ SDLK_l,               MLK_KEY_l                       },
	{ SDLK_m,               MLK_KEY_m                       },
	{ SDLK_n,               MLK_KEY_n                       },
	{ SDLK_o,               MLK_KEY_o                       },
	{ SDLK_p,               MLK_KEY_p                       },
	{ SDLK_q,               MLK_KEY_q                       },
	{ SDLK_r,               MLK_KEY_r                       },
	{ SDLK_s,               MLK_KEY_s                       },
	{ SDLK_t,               MLK_KEY_t                       },
	{ SDLK_u,               MLK_KEY_u                       },
	{ SDLK_v,               MLK_KEY_v                       },
	{ SDLK_w,               MLK_KEY_w                       },
	{ SDLK_x,               MLK_KEY_x                       },
	{ SDLK_y,               MLK_KEY_y                       },
	{ SDLK_z,               MLK_KEY_z                       },
	{ SDLK_CAPSLOCK,        MLK_KEY_CAPSLOCK                },
	{ SDLK_F1,              MLK_KEY_F1                      },
	{ SDLK_F2,              MLK_KEY_F2                      },
	{ SDLK_F3,              MLK_KEY_F3                      },
	{ SDLK_F4,              MLK_KEY_F4                      },
	{ SDLK_F5,              MLK_KEY_F5                      },
	{ SDLK_F6,              MLK_KEY_F6                      },
	{ SDLK_F7,              MLK_KEY_F7                      },
	{ SDLK_F8,              MLK_KEY_F8                      },
	{ SDLK_F9,              MLK_KEY_F9                      },
	{ SDLK_F10,             MLK_KEY_F10                     },
	{ SDLK_F11,             MLK_KEY_F11                     },
	{ SDLK_F12,             MLK_KEY_F12                     },
	{ SDLK_F13,             MLK_KEY_F13                     },
	{ SDLK_F14,             MLK_KEY_F14                     },
	{ SDLK_F15,             MLK_KEY_F15                     },
	{ SDLK_F16,             MLK_KEY_F16                     },
	{ SDLK_F17,             MLK_KEY_F17                     },
	{ SDLK_F18,             MLK_KEY_F18                     },
	{ SDLK_F19,             MLK_KEY_F19                     },
	{ SDLK_F20,             MLK_KEY_F20                     },
	{ SDLK_F21,             MLK_KEY_F21                     },
	{ SDLK_F22,             MLK_KEY_F22                     },
	{ SDLK_F23,             MLK_KEY_F23                     },
	{ SDLK_F24,             MLK_KEY_F24                     },
	{ SDLK_PRINTSCREEN,     MLK_KEY_PRINTSCREEN             },
	{ SDLK_SCROLLLOCK,      MLK_KEY_SCROLL_LOCK             },
	{ SDLK_PAUSE,           MLK_KEY_PAUSE                   },
	{ SDLK_INSERT,          MLK_KEY_INSERT                  },
	{ SDLK_HOME,            MLK_KEY_HOME                    },
	{ SDLK_PAGEUP,          MLK_KEY_PAGEUP                  },
	{ SDLK_DELETE,          MLK_KEY_DELETE                  },
	{ SDLK_END,             MLK_KEY_END                     },
	{ SDLK_PAGEDOWN,        MLK_KEY_PAGEDOWN                },
	{ SDLK_RIGHT,           MLK_KEY_RIGHT                   },
	{ SDLK_LEFT,            MLK_KEY_LEFT                    },
	{ SDLK_DOWN,            MLK_KEY_DOWN                    },
	{ SDLK_UP,              MLK_KEY_UP                      },
	{ SDLK_KP_DIVIDE,       MLK_KEY_KP_DIVIDE               },
	{ SDLK_KP_MULTIPLY,     MLK_KEY_KP_MULTIPLY             },
	{ SDLK_KP_MINUS,        MLK_KEY_KP_MINUS                },
	{ SDLK_KP_PLUS,         MLK_KEY_KP_PLUS                 },
	{ SDLK_KP_ENTER,        MLK_KEY_KP_ENTER                },
	{ SDLK_KP_1,            MLK_KEY_KP_1                    },
	{ SDLK_KP_2,            MLK_KEY_KP_2                    },
	{ SDLK_KP_3,            MLK_KEY_KP_3                    },
	{ SDLK_KP_4,            MLK_KEY_KP_4                    },
	{ SDLK_KP_5,            MLK_KEY_KP_5                    },
	{ SDLK_KP_6,            MLK_KEY_KP_6                    },
	{ SDLK_KP_7,            MLK_KEY_KP_7                    },
	{ SDLK_KP_8,            MLK_KEY_KP_8                    },
	{ SDLK_KP_9,            MLK_KEY_KP_9                    },
	{ SDLK_KP_0,            MLK_KEY_KP_0                    },
	{ SDLK_KP_PERIOD,       MLK_KEY_KP_PERIOD               },
	{ SDLK_KP_COMMA,        MLK_KEY_KP_COMMA                },
	{ SDLK_MENU,            MLK_KEY_MENU                    },
	{ SDLK_MUTE,            MLK_KEY_MUTE                    },
	{ SDLK_VOLUMEUP,        MLK_KEY_VOLUME_UP               },
	{ SDLK_VOLUMEDOWN,      MLK_KEY_VOLUME_DOWN             },
	{ SDLK_LCTRL,           MLK_KEY_LCTRL                   },
	{ SDLK_LSHIFT,          MLK_KEY_LSHIFT                  },
	{ SDLK_LALT,            MLK_KEY_LALT                    },
	{ SDLK_LGUI,            MLK_KEY_LSUPER                  },
	{ SDLK_RCTRL,           MLK_KEY_RCTRL                   },
	{ SDLK_RSHIFT,          MLK_KEY_RSHIFT                  },
	{ SDLK_RALT,            MLK_KEY_RALT                    },
	{ SDLK_RGUI,            MLK_KEY_RSUPER                  },
	{ 0,                    -1                              }
};

/* Maintain with enum mouse_button constants in mouse.h */
static const struct {
	int key;
	enum mlk_mouse_button value;
} buttons[] = {
	{ SDL_BUTTON_LEFT,      MLK_MOUSE_BUTTON_LEFT   },
	{ SDL_BUTTON_MIDDLE,    MLK_MOUSE_BUTTON_MIDDLE },
	{ SDL_BUTTON_RIGHT,     MLK_MOUSE_BUTTON_RIGHT  },
	{ -1,                   MLK_MOUSE_BUTTON_NONE   }
};

/* Maintain with enum mlk_gamepad_button in gamepad.h */
static const struct {
	int button;
	enum mlk_gamepad_button value;
} pads[] = {
	{ SDL_CONTROLLER_BUTTON_A,              MLK_GAMEPAD_BUTTON_A            },
	{ SDL_CONTROLLER_BUTTON_B,              MLK_GAMEPAD_BUTTON_B            },
	{ SDL_CONTROLLER_BUTTON_X,              MLK_GAMEPAD_BUTTON_X            },
	{ SDL_CONTROLLER_BUTTON_Y,              MLK_GAMEPAD_BUTTON_Y            },
	{ SDL_CONTROLLER_BUTTON_BACK,           MLK_GAMEPAD_BUTTON_BACK         },
	{ SDL_CONTROLLER_BUTTON_GUIDE,          MLK_GAMEPAD_BUTTON_LOGO         },
	{ SDL_CONTROLLER_BUTTON_START,          MLK_GAMEPAD_BUTTON_START        },
	{ SDL_CONTROLLER_BUTTON_LEFTSTICK,      MLK_GAMEPAD_BUTTON_LTHUMB       },
	{ SDL_CONTROLLER_BUTTON_RIGHTSTICK,     MLK_GAMEPAD_BUTTON_RTHUMB       },
	{ SDL_CONTROLLER_BUTTON_LEFTSHOULDER,   MLK_GAMEPAD_BUTTON_LSHOULDER    },
	{ SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,  MLK_GAMEPAD_BUTTON_RSHOULDER    },
	{ SDL_CONTROLLER_BUTTON_DPAD_UP,        MLK_GAMEPAD_BUTTON_UP           },
	{ SDL_CONTROLLER_BUTTON_DPAD_DOWN,      MLK_GAMEPAD_BUTTON_DOWN         },
	{ SDL_CONTROLLER_BUTTON_DPAD_LEFT,      MLK_GAMEPAD_BUTTON_LEFT         },
	{ SDL_CONTROLLER_BUTTON_DPAD_RIGHT,     MLK_GAMEPAD_BUTTON_RIGHT        },
	{ -1,                                   MLK_GAMEPAD_BUTTON_UNKNOWN      }
};

static void
convert_key(const SDL_Event *event, union mlk_event *ev)
{
	ev->type = event->type == SDL_KEYDOWN ? MLK_EVENT_KEYDOWN : MLK_EVENT_KEYUP;
	ev->key.key = MLK_KEY_UNKNOWN;

	for (size_t i = 0; keymap[i].key != 0; ++i) {
		if (keymap[i].key == event->key.keysym.sym) {
			ev->key.key = keymap[i].value;
			break;
		}
	}
}

static void
convert_mouse(const SDL_Event *event, union mlk_event *ev)
{
	ev->type = MLK_EVENT_MOUSE;
	ev->mouse.buttons = 0;
	ev->mouse.x = event->motion.x;
	ev->mouse.y = event->motion.y;

	if (event->motion.state & SDL_BUTTON_LMASK)
		ev->mouse.buttons |= MLK_MOUSE_BUTTON_LEFT;
	if (event->motion.state & SDL_BUTTON_MMASK)
		ev->mouse.buttons |= MLK_MOUSE_BUTTON_MIDDLE;
	if (event->motion.state & SDL_BUTTON_RMASK)
		ev->mouse.buttons |= MLK_MOUSE_BUTTON_RIGHT;
}

static void
convert_click(const SDL_Event *event, union mlk_event *ev)
{
	ev->type = event->type == SDL_MOUSEBUTTONDOWN ? MLK_EVENT_CLICKDOWN : MLK_EVENT_CLICKUP;
	ev->click.button = MLK_MOUSE_BUTTON_NONE;
	ev->click.x = event->button.x;
	ev->click.y = event->button.y;
	ev->click.clicks = event->button.clicks;

	for (size_t i = 0; buttons[i].value != MLK_MOUSE_BUTTON_NONE; ++i) {
		if (buttons[i].key == event->button.button) {
			ev->click.button = buttons[i].value;
			break;
		}
	}
}

static void
convert_pad(const SDL_Event *event, union mlk_event *ev)
{
	ev->type = event->type == SDL_CONTROLLERBUTTONDOWN ? MLK_EVENT_PADDOWN : MLK_EVENT_PADUP;

	for (size_t i = 0; pads[i].value != MLK_GAMEPAD_BUTTON_UNKNOWN; ++i) {
		if (pads[i].button == event->cbutton.button) {
			ev->pad.button = pads[i].value;
			break;
		}
	}
}

static const struct {
	int axis;
	enum mlk_gamepad_axis value;
} axises[] = {
	{ SDL_CONTROLLER_AXIS_LEFTX,            MLK_GAMEPAD_AXIS_LX             },
	{ SDL_CONTROLLER_AXIS_LEFTY,            MLK_GAMEPAD_AXIS_LY             },
	{ SDL_CONTROLLER_AXIS_RIGHTX,           MLK_GAMEPAD_AXIS_RX             },
	{ SDL_CONTROLLER_AXIS_RIGHTY,           MLK_GAMEPAD_AXIS_RY             },
	{ SDL_CONTROLLER_AXIS_TRIGGERLEFT,      MLK_GAMEPAD_AXIS_LTRIGGER       },
	{ SDL_CONTROLLER_AXIS_TRIGGERRIGHT,     MLK_GAMEPAD_AXIS_RTRIGGER       },
	{ -1,                                   MLK_GAMEPAD_AXIS_UNKNOWN        }
};

static void
convert_axis(const SDL_Event *event, union mlk_event *ev)
{
	ev->type = MLK_EVENT_AXIS;
	ev->axis.value = event->caxis.value;

	for (size_t i = 0; axises[i].value != MLK_GAMEPAD_AXIS_UNKNOWN; ++i) {
		if (axises[i].axis == event->caxis.axis) {
			ev->axis.axis = axises[i].value;
			break;
		}
	}
}

int
mlk_event_poll(union mlk_event *ev)
{
	SDL_Event event;

	memset(ev, 0, sizeof (*ev));

	/*
	 * Loop until we find an event we want to report, we skip unneeded
	 * ones.
	 */
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			ev->type = MLK_EVENT_QUIT;
			return 1;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			convert_key(&event, ev);
			return 1;
		case SDL_MOUSEMOTION:
			convert_mouse(&event, ev);
			return 1;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			convert_click(&event, ev);
			return 1;
		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERBUTTONUP:
			convert_pad(&event, ev);
			return 1;
		case SDL_CONTROLLERAXISMOTION:
			convert_axis(&event, ev);
			return 1;
		default:
			continue;
		}
	}

	return 0;
}
