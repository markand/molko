# Module: sound

Synopsis

```c
#include <core/sound.h>
```

This module provides sound support. Sounds differ from [musics](music.md)
because several sounds can be played at a time using channels.

## Macros

### SOUND\_CHANNEL\_MAX

Number of channels allocated.

```c
#define SOUND_CHANNELS_MAX (256)
```

## Structs

### sound

This structure has no public editable fields but is still publicly exposed to
allow stack allocation.

## Functions

### sound\_open

Open a sound file from `path` and store the result into `snd`. Returns false on
errors, in this case `snd` remains uninitialized and must not be used.

```c
bool
sound_open(struct sound *snd, const char *path)
```

### sound\_openmem

Open a sound from the memory `buffer` of size `buffersz` and store the result
into `snd`. Returns false on errors, in this case `snd` remains uninitialized
and must not be used.

!!! note
    The argument `buffer` must stay valid until the sound is no longer used.

```c
bool
sound_openmem(struct sound *snd, const void *buffer, size_t buffersz)
```

### sound\_ok

Returns true if the sound `snd` is properly initialized.

```c
bool
sound_ok(const struct sound *snd)
```

### sound\_play

Start playing the sound `snd` on the given `channel` (or -1 to pick one). If the
channel is currently playing, its playback is stopped.

The optional argument `fadein` can create a fade in effect expressed in
milliseconds.

```c
bool
sound_play(struct sound *snd, int channel, unsigned int fadein)
```

### sound\_pause

Pause the sound `snd` playback or all if set to NULL.

```c
void
sound_pause(struct sound *snd)
```

### sound\_resume

Resume the sound `snd` playback or all if set to NULL.

```c
void
sound_resume(struct sound *snd)
```

### sound\_stop

Stop the sound `snd` playback or all if set to NULL.

The optional argument `fadeout` can create a fade out effect expressed in
milliseconds.

```c
void
sound_stop(struct sound *snd, unsigned int fadeout)
```

### sound\_finish

Close the sound `snd`, if it is currently playing it will immediately stops.

```
void
sound_finish(struct sound *snd)
```
