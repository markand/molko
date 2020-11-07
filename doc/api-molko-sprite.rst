.. toctree::
.. highlight:: js
.. _api-molko-sprite:

============
Molko.Sprite
============

The sprite module let you draw specific regions of grid based textures with
ease.

Constructors
------------

.. js:function:: Molko.Sprite(texture, cellw, cellh)

   :param Texture texture: The texture to use.
   :param uint cellw: Per cell width.
   :param uint cellh: Per cell height.

Construct a sprite with the given texture and the cell grid dimensions. This
function MUST be called as a constructor, not doing so will also raise an
error.

Methods
-------

.. js:method:: Molko.Sprite.draw(object)
               Molko.Sprite.draw(row, column, x, y)

  :param Object object: Description.
  :param uint row: Row number.
  :param uint column: Column number.
  :param int x: Position in x.
  :param int y: Position in y.

Draw the given row/column cell from the sprite onto the screen. Two signatures
are allowed.

In the second form, each argument is passed individually, in the first form it
it takes an object with the properties having the same name as individual
arguments.

Example:

::

  sprite.draw(1, 2, 100, 100);
  sprite.draw({row: 1, column: 2, x: 100, y: 100});

Properties
----------

.. js:attribute:: Molko.Sprite.rowCount

(Read-only) Number of rows calculated from image dimension.

.. js:attribute:: Molko.Sprite.columnCount

(Read-only) Number of columns calculated from image dimension.
