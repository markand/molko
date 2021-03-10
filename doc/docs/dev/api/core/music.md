# Module: music

Synopsis

```c
#include <core/music.h>
```

This module provide support for playing music. In contrast to [sounds](sound.md)
only one music can be played at a time.

## Enums

### music\_flags

Optional music flags that can be OR'ed in functions which use them.

| Enumerator   | Description     |
|--------------|-----------------|
| `MUSIC_NONE` | No flags.       |
| `MUSIC_LOOP` | Loop the music. |

## Structs

### music

This structure has no public editable fields but is still publicly exposed to
allow stack allocation.

## Functions

### music\_open

Open a music file from `path` and store the result into `mus`.

Returns -1 in case of errors and 0 otherwise.

```c
int
music_open(struct music *mus, const char *path)
```

### music\_openmem

Open a music from the memory `buffer` of size `buffersz` and store the result
into `mus`.

Returns -1 in case of errors and 0 otherwise.

!!! note
    The argument `buffer` must stay valid until the music is no longer used.

```c
int
music_openmem(struct music *mus, const void *buffer, size_t buffersz)
```

### music\_ok

Returns non-zero if the music `mus` is properly initialized.

```c
int
music_ok(const struct music *mus)
```

### music\_play

Start playing the given music `mus`.

The argument `flags` can control the playback options and if `fadein` is greater
than 0 it will fade in during the number of specified milliseconds.

This function will resume the playback since the beginning and will stop the
current music. If the music playing is currently fading out the playback will
not start until it has finished.

Returns -1 in case of errors and 0 otherwise.

```
int
music_play(struct music *mus, enum music_flags flags, unsigned int fadein)
```

### music\_playing

Returns non-zero if a music is playing.

```c
int
music_playing(void)
```

### music\_pause

Pause the music playback immediately.

```c
void
music_pause(void)
```

### music\_resume

Resume the music playback immediately.

```c
void
music_resume(void)
```

### music\_stop

Stop the music playback, applying an optional `fadeout` delay expressed in
milliseconds.

```c
void
music_stop(unsigned int fadeout)
```

### music\_finish

Close the music `mus`, if it is playing it is immediately stopped.

```c
void
music_finish(struct music *mus)
```
