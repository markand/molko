.. toctree::
.. highlight:: javascript
.. _api-molko-music:

Molko.Music
===========

Module for playing music. In contrast to sounds, you may play only one music at
a time. On the other hand, a music can be looped.

Constants
---------

.. js:data:: Molko.Music.<FLAG>

The following values are available into the object itself:

``NONE``
  No flags to apply.
``LOOP``
  Loop the music.

Constructors
------------

.. js:function:: Molko.Music(path)

   :param string path: Path to the music file.
   :throws Error: If unable to open the file.

Open the music from the given file.

Functions
---------

Since only one music can be played at a time, those following functions are
static in the class.

.. js:function:: Molko.Music.playing()

   :returns: True if a music is playing.

Tells if any music is currently playing.

.. js:function:: Molko.Music.pause()

Pause music playback immediately.

.. js:function:: Molko.Music.resume()

Resume music playback.

.. js:function:: Molko.Music.stop(fadeout = 0)

   :param uint fadeout: Optional fade out in milliseconds.

Stop the music playback and apply an optional fade out effect.

Methods
-------

.. js:method:: Molko.Music.play(effect = Molko.Music.NONE, fadein = 0)

   :param uint effect: Optional flags (see :js:data:`Molko.Music.\<FLAG\>`).
   :param uint fadein: Optional fade in in milliseconds.
   :throws Error: In case of failure.

Start playing this music with an optional fade in delay. If there is already a
music playing it is stopped immediately unless it is currently fading out.

Examples
--------

Play a loop music with a small 1/2 second fade in effect.

::

  var m = new Molko.Music("sample.ogg");

  m.play(Molko.Music.LOOP, 500);
