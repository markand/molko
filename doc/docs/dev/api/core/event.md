# Module: event

Synopsis

```c
#include <core/event.h>
```

Poll user input and system events.

## Enums

### event\_type

Which kind of event has been reported.

| Enumerator        | Description                                           |
|-------------------|-------------------------------------------------------|
| `EVENT_CLICKDOWN` | Mouse click down, see [event_click](#event_click)     |
| `EVENT_CLICKUP`   | Mouse click released, see [event_click](#event_click) |
| `EVENT_KEYDOWN`   | Single key down, see [event_key](#event_key)          |
| `EVENT_KEYUP`     | Single key released, see [event_key](#event_key)      |
| `EVENT_MOUSE`     | Mouse moved, see [event_mouse](#event_mouse)          |
| `EVENT_QUIT`      | Quit request                                          |

## Unions

### event

Union that store every kind of event.

The union along with every sub structures that are defined all have the `enum
event_type` as first field which means that you can access the union's type
field directly.

Depending on the [type](#type) field, access only the appropriate sub structure
that describe the event. See below structures that describe events.

| Field           | Access | Type                 |
|-----------------|--------|----------------------|
| [type](#type)   | (-)    | `enum event_type`    |
| [key](#key)     | (+)    | `struct event_key`   |
| [mouse](#mouse) | (+)    | `struct event_mouse` |
| [click](#click) | (+)    | `struct event_click` |

#### type

Which kind of event happened.

#### key, mouse, click

Access to the element details depending on the type.

## Structs

### event\_key

Describe a keyboard key that was pressed or released.

| Field           | Access | Type              |
|-----------------|--------|-------------------|
| [type](#type_1) | (-)    | `enum event_type` |
| [key](#key)     | (+)    | `enum key`        |

#### type

Set to `EVENT_KEYDOWN` or `EVENT_KEYUP`.

#### key

Which key, see [key](key.md) for more information.

### event\_mouse

Describe a mouse motion.

| Field               | Access | Type                |
|---------------------|--------|---------------------|
| [type](#type_2)     | (-)    | `enum event_type`   |
| [buttons](#buttons) | (+)    | `enum mouse_button` |
| [x](#x-y)           | (+)    | `int`               |
| [y](#y-y)           | (+)    | `int`               |

#### type

Set to `EVENT_MOUSE`.

#### buttons

OR'ed values of buttons currently pressed, see [mouse](mouse.md) for more
information.

#### x, y

New absolute coordinates.

### event\_click

Describe a mouse click or release.

| Field             | Access | Type                |
|-------------------|--------|---------------------|
| [type](#type_2)   | (-)    | `enum event_type`   |
| [button](#button) | (+)    | `enum mouse_button` |
| [x](#x-y_1)       | (+)    | `int`               |
| [y](#y-y_1)       | (+)    | `int`               |

#### type

Set to `EVENT_CLICKDOWN` or `EVENT_CLICKUP`.

#### button

Unique button pressed or released.

#### x, y

Absolute coordinates.

## Functions

### event\_poll

Fetch the next event into `ev` or returns 0 if there are none.

```c
int
event_poll(union event *ev)
```
