# Module: inhibit

Synopsis

```c
#include <core/inhibit.h>
```

Inhibit some mechanism of the game loop.

This module is mostly use only for the [game](game.md) module.

!!! hint
    This module should be used as last resort because it may introduce bugs if
    you forget to reset inhibit. In most of the cases you would want to use
    [actions](action.md) and block a state until it's complete.

## Enums

### inhibit

This enumeration contains values than can be OR'ed and stored in
[game.inhibit](game.md#inhibit) variable.

| Enumerator             | Description           |
|------------------------|-----------------------|
| `INHIBIT_NONE`         | Disable nothing.      |
| `INHIBIT_STATE_INPUT`  | Disable every events. |
| `INHIBIT_STATE_UPDATE` | Disable update.       |
| `INHIBIT_STATE_DRAW`   | Disable draw.         |

!!! caution
    Use `INHIBIT_STATE_DRAW` with care because not re-rendering the screen can
    cause artifacts.
