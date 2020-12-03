# Module: key

Synopsis

```c
#include <core/key.h>
```

Keyboard definitions.

## Enums

### key

This enumeration contains keys supported by the API.

!!! caution
    Many keys are not portable or may be not available on every keyboards. For
    example, it's pretty common to not see some keys on laptops which has much
    smaller keyboards. Avoid use of non “simple” keys in your game play.

| Enumerator            | Description                   |
|-----------------------|-------------------------------|
| `KEY_UNKNOWN`         | Unknown key                   |
| `KEY_ENTER`           | Return                        |
| `KEY_ESCAPE`          | Escape                        |
| `KEY_BACKSPACE`       | Backspace                     |
| `KEY_TAB`             | Tab                           |
| `KEY_SPACE`           | Space                         |
| `KEY_EXCLAIM`         | !                             |
| `KEY_DOUBLE_QUOTE`    | "                             |
| `KEY_HASH`            | #                             |
| `KEY_PERCENT`         | %                             |
| `KEY_DOLLAR`          | $                             |
| `KEY_AMPERSAND`       | %                             |
| `KEY_QUOTE`           | '                             |
| `KEY_LEFT_PAREN`      | (                             |
| `KEY_RIGHT_PAREN`     | )                             |
| `KEY_ASTERISK`        | \*                            |
| `KEY_PLUS`            | + (top row)                   |
| `KEY_COMMA`           | ,                             |
| `KEY_MINUS`           | - (top row)                   |
| `KEY_PERIOD`          | .                             |
| `KEY_SLASH`           | /                             |
| `KEY_0`               | (top row)                     |
| `KEY_1`               | (top row)                     |
| `KEY_2`               | (top row)                     |
| `KEY_3`               | (top row)                     |
| `KEY_4`               | (top row)                     |
| `KEY_5`               | (top row)                     |
| `KEY_6`               | (top row)                     |
| `KEY_7`               | (top row)                     |
| `KEY_8`               | (top row)                     |
| `KEY_9`               | (top row)                     |
| `KEY_COLON`           | :                             |
| `KEY_SEMICOLON`       | ;                             |
| `KEY_LESS`            | <                             |
| `KEY_EQUALS`          | =                             |
| `KEY_GREATER`         | >                             |
| `KEY_QUESTION`        | ?                             |
| `KEY_AT`              | @                             |
| `KEY_LEFT_BRACKET`    | [                             |
| `KEY_BACKSLASH`       | \                             |
| `KEY_RIGHT_BRACKET`   | ]                             |
| `KEY_CARET`           | ^                             |
| `KEY_UNDERSCORE`      | \_                            |
| `KEY_BACKQUOTE`       | \`                            |
| `KEY_a`               |                               |
| `KEY_b`               |                               |
| `KEY_c`               |                               |
| `KEY_d`               |                               |
| `KEY_e`               |                               |
| `KEY_f`               |                               |
| `KEY_g`               |                               |
| `KEY_h`               |                               |
| `KEY_i`               |                               |
| `KEY_j`               |                               |
| `KEY_k`               |                               |
| `KEY_l`               |                               |
| `KEY_m`               |                               |
| `KEY_n`               |                               |
| `KEY_o`               |                               |
| `KEY_p`               |                               |
| `KEY_q`               |                               |
| `KEY_r`               |                               |
| `KEY_s`               |                               |
| `KEY_t`               |                               |
| `KEY_u`               |                               |
| `KEY_v`               |                               |
| `KEY_w`               |                               |
| `KEY_x`               |                               |
| `KEY_y`               |                               |
| `KEY_z`               |                               |
| `KEY_CAPSLOCK`        | Caps lock                     |
| `KEY_F1`              |                               |
| `KEY_F2`              |                               |
| `KEY_F3`              |                               |
| `KEY_F4`              |                               |
| `KEY_F5`              |                               |
| `KEY_F6`              |                               |
| `KEY_F7`              |                               |
| `KEY_F8`              |                               |
| `KEY_F9`              |                               |
| `KEY_F10`             |                               |
| `KEY_F11`             |                               |
| `KEY_F12`             |                               |
| `KEY_F13`             | (not portable)                |
| `KEY_F14`             | (not portable)                |
| `KEY_F15`             | (not portable)                |
| `KEY_F16`             | (not portable)                |
| `KEY_F17`             | (not portable)                |
| `KEY_F18`             | (not portable)                |
| `KEY_F19`             | (not portable)                |
| `KEY_F20`             | (not portable)                |
| `KEY_F21`             | (not portable)                |
| `KEY_F22`             | (not portable)                |
| `KEY_F23`             | (not portable)                |
| `KEY_F24`             | (not portable)                |
| `KEY_PRINTSCREEN`     | Print screen                  |
| `KEY_PAUSE`           | Media pause                   |
| `KEY_INSERT`          | Insert (not portable)         |
| `KEY_HOME`            | Home (not portable)           |
| `KEY_PAGEUP`          | Page up                       |
| `KEY_DELETE`          | Back delete                   |
| `KEY_END`             | End                           |
| `KEY_PAGEDOWN`        | Page down                     |
| `KEY_RIGHT`           | Right arrow                   |
| `KEY_LEFT`            | Left arrow                    |
| `KEY_DOWN`            | Down arrow                    |
| `KEY_UP`              | Up arrow                      |
| `KEY_KP_DIVIDE`       | / (keypad)                    |
| `KEY_KP_MULTIPLY`     | * (keypad)                    |
| `KEY_KP_MINUS`        | - (keypad)                    |
| `KEY_KP_PLUS`         | + (keypad)                    |
| `KEY_KP_ENTER`        | Return (keypad)               |
| `KEY_KP_1`            | (keypad)                      |
| `KEY_KP_2`            | (keypad)                      |
| `KEY_KP_3`            | (keypad)                      |
| `KEY_KP_4`            | (keypad)                      |
| `KEY_KP_5`            | (keypad)                      |
| `KEY_KP_6`            | (keypad)                      |
| `KEY_KP_7`            | (keypad)                      |
| `KEY_KP_8`            | (keypad)                      |
| `KEY_KP_9`            | (keypad)                      |
| `KEY_KP_0`            | (keypad)                      |
| `KEY_KP_PERIOD`       | . (keypad)                    |
| `KEY_KP_COMMA`        | , (keypad)                    |
| `KEY_MENU`            | Menu key (not portable)       |
| `KEY_MUTE`            | Volumne mute (not portable)   |
| `KEY_VOLUME_UP`       | Volume up (not portable)      |
| `KEY_VOLUME_DOWN`     | Volume down (not portable)    |
| `KEY_LCTRL`           | Left control                  |
| `KEY_LSHIFT`          | Left shift                    |
| `KEY_LALT`            | Left alt                      |
| `KEY_LSUPER`          | Left super (or logo)          |
| `KEY_RCTRL`           | Right control                 |
| `KEY_RSHIFT`          | Right shift                   |
| `KEY_RALT`            | Right alt                     |
| `KEY_RSUPER`          | Right super (or logo)         |

### keymod

This enumeration contains keyboards modifiers that can be OR'ed because you may
press them more than one at once.

| Enumerator      | Description        |
|-----------------|--------------------|
| `KEYMOD_LSHIFT` | Left shift         |
| `KEYMOD_LCTRL`  | Left control       |
| `KEYMOD_LALT`   | Left alt           |
| `KEYMOD_LSUPER` | Left super (logo)  |
| `KEYMOD_RSHIFT` | Right shift        |
| `KEYMOD_RCTRL`  | Right control      |
| `KEYMOD_RALT`   | Right alt          |
| `KEYMOD_RSUPER` | Right super (logo) |
