.. toctree::
.. _api-molko-painter:

Molko.Painter
=============

Module for drawing operation into the screen or a texture. It is a low level
module with primitives operations.

You must open a window before using this module. See :ref:`api-molko-window`.

Constructors
------------

.. js:function:: Molko.Painter()

Create a painter object. This function MUST be called as a constructor, not
doing so will also raise an error.

Methods
-------

.. js:method:: Molko.Painter.clear()

Clear the rendering target with the current color.

.. js:method:: Molko.Painter.drawLine(line)
               Molko.Painter.drawLine(x1, x2, x2, y2)

   :param object line: A line segment.
   :param int x1: Start position in x.
   :param int y1: Start position in y.
   :param int x2: End position in x.
   :param int y2: End position in y.

Draw a line, two signatures are allowed.

In the second form, each argument is passed individually, in the first form it
it takes an object with the following properties:

``x1 (int)``
  Start position in x.
``y1 (int)``
  Start position in y.
``x2 (int)``
  End position in x.
``y2 (int)``
  End position in y.

.. js:method:: Molko.Painter.drawPoint(point)
               Molko.Painter.drawPoint(x, y)

   :param object point: A point.
   :param int x: Start position in x.
   :param int y: Start position in y.

Draw a unique point, two signatures are allowed.

In the second form, each argument is passed individually, in the first form it
it takes an object with the following properties:

``x (int)``
  Position in x.
``y (int)``
  Position in y.

.. js:method:: Molko.Painter.drawRectangle(rectangle)
               Molko.Painter.drawRectangle(x, y, w, h)

   :param object rectangle: A rectangle.
   :param int x: Position in x.
   :param int y: Position in y.
   :param int w: Rectangle width.
   :param int h: Height width.

Fill a rectangle region, two signatures are allowed.

In the second form, each argument is passed individually, in the first form it
it takes an object with the following properties:

``x (int)``
  Position in x.
``y (int)``
  Position in y.
``w (int)``
  Rectangle width.
``h (int)``
  Height width.

.. js:method:: Molko.Painter.drawCircle(circle)
               Molko.Painter.drawCircle(x, y, r)

   :param object circle: A circle.
   :param int x: Position in x.
   :param int y: Position in y.
   :param number radius: Radius.

Draw a circle, two signatures are allowed.

In the second form, each argument is passed individually, in the first form it
it takes an object with the following properties:

``x (int)``
  Position in x.
``y (int)``
  Position in y;
``radius (number)``
  Radius.

.. js:method:: Molko.Painter.present()

Present the rendered operations to the screen.

Properties
----------

.. js:attribute:: Molko.Painter.color

(Read-write) Color for next drawing operation.
