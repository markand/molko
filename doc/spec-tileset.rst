.. toctree::
.. _spec-tileset:

=============
Spec: tileset
=============

Tilesets are files that are used to draw a map by copying individual cells from
an image into the screen by repeating those. A unique tileset can be reused by
several maps to avoid recreating every collision and animations each time a new
map is created.

They can be used directly as C code but can be written and read from files as
well.

Tilesets consists of:

- An image to use as a sprite,
- Dimensions of individual cells in that image,
- Set of animations for specific tiles,
- Set of collision masks for specific tiles.

Tilesets have an extension of ``.tileset``.

File format
-----------

The file format is a simple plain text where each each line consist of a
directive.

.. caution:: Order is important. You must make sure that every directives are
   listed in the same order.

The file format must be defined as following where each ``<>`` must be replaced
by the appropriate C type name.

.. code-block:: text

  tilewidth|<unsigned int>
  tileheight|<unsigned int>
  image|<const char *>
  tiledefs
  <unsigned short>|<int>|<int>|<unsigned int>|<unsigned int>
  ...
  animations
  <unsigned short>|<const char *>|<unsigned int>

Description of directives:

``tilewidth``
  Cell width in the image.
``tileheight``
  Cell height in the image.
``image``
  Path to image to use (must be relative).
``tiledefs``
  Start a list of tile definitions to describe a collision mask for a given
  tile. Each line after this directive describe a unique rectangle for one tile
  by specifying ``tile id``, ``x``, ``y``, ``width``, ``height`` in order.

  This section is optional and can be omitted entirely.
``animations``
  Start a list of tile animations that should be used instead of drawing from
  the image sprite itself. Each line after this directive describe an animation
  for a given tile by specifying ``tile id``, ``animation file``, ``delay`` in
  order.

Examples
--------

The following file is a tileset using the image *world.png* that has 64x64
dimensions for each cell.

.. code-block:: text

  tilewidth|64
  tileheight|64
  image|world.png

The following file adds some collision rectangles on tiles *3* and *5*. The
rectangle collision for the tile id *3* has dimensions of W=30, H=30, X=5, Y=5
and the rectangle for the tile id *5* has dimensions of W=10, H=15, X=10, Y=10.

.. code-block:: text

  tilewidth|64
  tileheight|64
  image|world.png
  tiledefs
  3|30|30|5|5
  5|10|15|10|10

The following file adds an animation for the tile id *3* using the file
*animation-water.png* with a delay of *500* ms per tile.

.. code-block:: text

  tilewidth|64
  tileheight|64
  image|world.png
  animations
  3|animation-water.png|500
