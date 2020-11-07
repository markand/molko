.. toctree::
.. _api-molko-event:

Molko.Event
===========

Poll user and kernel events from the main loop.

.. caution::
   Not polling events in the main loop may end in an unresponsive application on
   some platforms.

Constants
---------

.. js:data:: Molko.Event

The following values are available into the object itself:

``CLICK_DOWN``
  Mouse click down.
``CLICK_UP``
  Mouse click release.
``KEY_DOWN``
  Keyboard key down.
``KEY_UP``
  Keyboard key release.
``MOUSE``
  Mouse motion.
``QUIT``
  User quit event.

Functions
---------

.. js:function:: Molko.Event.poll()

   :returns: An object or undefined if there are no more events.

Get the next pending event in the queue. It is recommended to use this function
in a loop to unqueue the whole list of events at once for performance reasons.

The returned object has different properties depending on the event.

For both ``CLICK_DOWN`` and ``CLICK_UP`` events:

``x (int)``
  Position in x.
``y (int)``
  Position in y.
``button (int)``
  Button number (1 through 5 are standards).

For both ``KEY_DOWN`` and ``KEY_UP`` events:

``key (int)``
  Key number.

For ``MOUSE`` event:

``x (int)``
  Position in x.
``y (int)``
  Position in y.
``buttons (int)``
  Bitmask of buttons currently pressed.
