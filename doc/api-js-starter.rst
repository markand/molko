.. highlight:: javascript

================
Starter template
================

In this guide we will show you how to start developing. Open your favorite
editor and create a Javascript file, in this example we will use ``main.js`` but
the name does not matter.

Opening a window
----------------

The very first thing to do is to open a window. To do this we use the
:js:mod:`Molko.Window`. The function we are interested in is the window
constructor :js:func:`Molko.Window.new`, it takes a title and a dimension.

.. hint::
   Altough the Javascript API does not expect a specific window size, the
   default predefined objects make best fit using a resolution of 720p
   (1280x720).

Create a window::

  var w = new Molko.Window("My first window", 1280, 720);

Basic event handling
--------------------

Now that a window is opened, we need to dispatch user and events from the
operating system to run and be able to draw our game to the screen.

.. caution::
   You must use this API at some point in your code otherwise your application
   may look unresponsive on some platforms.

To do this, we create an infinite loop and then at the beginning of our loop we
will poll all events until it is empty. This is done with the
:js:mod:`Molko.Event` API.

::

  var run = true;

  while (run) {
    for (var ev; ev = Molko.Event.poll(); ev) {
      switch (ev.type) {
      case Molko.Event.QUIT:
        run = false;
        break;
      default:
        break;
      }
    }
  }

Rendering
---------

Our window is now ready to render things to the user. For this we may use
several APIs like :js:mod:`Molko.Texture`, :js:mod:`Molko.Painter` and such. We
will focus on the latter for now.

.. caution::
   You must create a window before using a painter.

In this example, we will clear the black in screen and draw a small square in
the middle of the screen.

::

  var p = new Molko.Painter()

  // Clear the screen.
  p.color = 0x000000ff;
  p.clear();
  p.color = 0xffffffff;
  p.drawRectangle((1280 - 50) / 2, (720 - 50) / 2, 50, 50);
  p.present();

Final word
----------

That's it for the core API! You are now able to run a small script that clear
the screen and catch user quit event.

This is the whole code or our file:

::

  var w = new Molko.Window("My first window", 1280, 720);
  var p = new Molko.Painter()

  var run = true;

  while (run) {
    // Poll pending events.
    for (var ev; ev = Molko.Event.poll(); ev) {
      switch (ev.type) {
      case Molko.Event.QUIT:
        run = false;
        break;
      default:
        break;
      }
    }

    // Clear the screen.
    p.color = 0x000000ff;
    p.clear();
    p.color = 0xffffffff;
    p.drawRectangle((1280 - 50) / 2, (720 - 50) / 2, 50, 50);
    p.present();
  }
