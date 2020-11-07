.. toctree::
.. _api-molko-window:

Molko.Window
============

This module is dedicated to window management.

Constants
---------

.. js:data:: Molko.Window.Cursor

This is an enumeration of available cursor which you may choose from:

``ARROW``
  Default arrow.

``EDIT``
  Edit text cusror (like I-Beam)

``WAIT``
  Busy cursor, not available on all platforms.

``CROSS_HAIR``
  Cross.

``SIZE``
  Move and resizing cursor.

``NO``
  Action forbidden.

``HAND``
  Hand, mostly for moving.

Constructors
------------

.. js:function:: Molko.Window(title, width, height)

   :param string title: Title for the window.
   :param int width: Desired window width.
   :param int height: Desired window height.

Open a window or raise an error if any. This function MUST be called as a
constructor, not doing so will also raise an error.

Properties
----------

.. js:data:: Molko.Window.cursor

(Write-only) Change the global window cursor to a system predefined one.
