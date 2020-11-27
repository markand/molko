# Howto: ownership and memory management

How memory and ownership is used within Molko's Adventure API.

## Synopsis

In C, memory management can be a cumbersome issue and a major common complain
when it comes to leak.

As such, the API itself does not use dynamic allocations in most of the case.
Instead it is kept to the user responsability to handle storage of data. In that
manner the code stays simpler, stronger and more flexible. The API itself does
not have to deal with memory management and deallocation, it only expect data
from user and work with that.

You can imagine a situation with a DVD player and some movies on DVDs. You have
one DVD player where you put some DVDs inside but the DVD player never make its
own copy, it simply reads your disc and you get it back afterwards.

Following this philosophy, the Molko's Adventure API for this scenario would
look like this:

```c
struct dvd_player player;
struct dvd dvd;

dvd_open(&dvd, "/dev/sr0");

dvd_player_turn_on();
dvd_player_insert(&player, &dvd);
dvd_player_play(&player);
dvd_player_turn_off();
```

## Memory handling

### Arrays

Some modules in the API may require an array. Depending on the situation they
can be passed as parameter or are usually of a fixed reasonable size in
structures.

They are always annotated with a macro to let the user flexibility over it if
necessary.

Example, a player has a set of spells.

```c
#define PLAYER_SPELL_MAX (16)

struct player {
    struct spell *spells[PLAYER_SPELL_MAX];
};
```

!!! note
    You may think that we could use a pointer to pointer of spell to let user
    pass a variable number of spells. That would help flexibility but makes user
    code more complex and painful, instead the macro can be redefined at build
    time.

### Strings

When dealing with strings, they are almost always referenced and not copied
unless explicitly required. As such, no allocation/deallocation required either.

```c
struct player {
    const char *name; // Not allocated, no deallocation
```

## Ownership

Alongside the memory handling comes the ownership. Related to the DVD scenario
explained above, not having to *own* a resource within a structure means no
allocation/deallocation required.

Also, since C does not have a scoped mechanism, all fields in structured are
publicly available and to avoid allocating them on the heap they are always
declared to the user even if they need internal data.

As a documentation notice, fields are always annotated using (XYZ) prefix with
some symbols to indicate whether user is allowed to touch or not.

The letter *X* defines the following restriction

`+`
:   The property is readable/editable by the user,

`-`
:   The property is readable by the user,

`*`
:   The property is not meant to be used directly by the user.

The letter *Y* can be set to `&` in that case means it is only referenced and is
not an owned property (usually a non-owning pointer). Which means user is
responsible for deallocation if required.

The finall letter *Z* can be set to `?` which means it is optional (like a
nullable pointer).

Examples:

```c
struct foo {
    int x;                /*!< (+) Position in x. */
    int y;                /*!< (+) Position in y. */
    struct theme *th;     /*!< (+&?) Theme to use. */
    unsigned int elapsed; /*!< (-) Elapsed time since last frame. */
    struct texture text;  /*!< (*) Texture used for rendering. */
};
```

Within this structure, the fields `x` and `y` are completely accessible to the
user for both reading/writing. The field `th` is an optional non-owning pointer
to a theme which is also readable/writable. The field `elapsed` is readable but
should not be modified. Finally, the field `text` is private and should not be
touched by the user.

## Memory handling in Molko's Adventure API

|         | Dynamic allocation? | Notes                                                   |
|---------|---------------------|---------------------------------------------------------|
| libcore | None                | The util.h provides convenient allocators for the user. |
| libui   | None                |                                                         |
| librpg  | In map.h module     | Maps are big chunk of data.                             |
