# Module: animation

Synopsis

```c
#include <core/animation.h>
```

Drawable animations.

Animations are small objects using a [sprite](sprite.md) to update itself and
draw next frames depending on delay set.

## Structs

### animation

| Field               | Access | Type              |
|---------------------|--------|-------------------|
| [sprite](#sprite)   | (+&)   | `struct sprite *` |
| [row](#row)         | (+)    | `unsigned int`    |
| [column](#column)   | (+)    | `unsigned int`    |
| [delay](#delay)     | (+)    | `unsigned int`    |
| [elapsed](#elapsed) | (-)    | `unsigned int`    |

#### sprite

The [sprite](sprite.md) file to use.

#### row

Current row to be shown.

#### column

Current column to be shown.

#### delay

Delay between each frame in milliseconds.

#### elapsed

Elapsed time since last frame.

## Functions

### animation\_init

Initialize the animation `an` with the sprite `sprite` using a `delay` between
each frame.

!!! note
    This function can be omitted if the object is set using designated
    initializers (setting other fields to 0).

```c
void
animation_init(struct animation *an, struct sprite *sprite, unsigned int delay)
```

### animation\_start

Start or reset the animation `an` to the beginning.

```c
void
animation_start(struct animation *an)
```

### animation\_completed

Returns non-zero if the animation `an` was completely shown.

```c
int
animation_completed(const struct animation *an)
```

### animation\_update

Update the animation `an` with `ticks` since last frame. Returns non-zero if it
has completed.

```c
int
animation_update(struct animation *an, unsigned int ticks)
```

### animation\_draw

Draw the animation `an` to the given `x`, `y` coordinates.

Returns -1 in case of errors and 0 otherwise.

!!! warning
    You must not call this function is the animation is complete.

```c
int
animation_draw(const struct animation *an, int x, int y)
```

### animation\_drawable

Fill the `dw` drawable with functions to draw the animation `an` at the
coordinates `x`, `y`.

!!! important
    The animation is only borrowed and must be kept valid during the whole `dw`
    lifetime.

```c
void
animation_drawable(struct animation *an, struct drawable *dw, int x, int y)
```

## Examples

### Initialization

Initialize using designated initializers

```c
struct animation an = {
	.sprite = &my_sprite,
	.delay = 50
};
```

Using `animation_init`

```c
struct animation an;

animation_init(&an, &my_sprite, 50);
```

### Updating and drawing

A simple animation loop should like this:

```c
struct animation an = {
	.sprite = &my_sprite,
	.delay = 50
};

for (;;) {
	/* Compute ticks in your game loop frame time. */
	unsigned int ticks = UserTicks();

	if (!animation_update(&an, ticks))
		animation_draw(&an, 100, 150);
}
```
