.. toctree::
.. _api-molko-font:

Molko.Font
==========

.. js:module:: Molko.Font

Module for loading fonts and rendering text with them.

Functions
---------

.. js:function:: Molko.Font.fromPath(path, size)

   :param string path: Path to the font to load.
   :param size int: Desired size.
   :throws: Error in case of loading error.
   :returns: A font handle.

Open the font specified by path

Methods
-------

.. js:method:: Molko.Font.query(text)

   :param string text: Text to use for computing size.
   :throws: Error in case of error.
   :returns: An object with w, h properties.

Get the required bounding box size to render the given text. The returned object
has the following properties:

``w (int)``
  The text's width.
``h (int)``
  The text's height.

.. js:method:: Molko.Font.render(text, color)

   :param string text: Text to render.
   :param number color: The color.
   :throws: Error in case of error.
   :returns: A texture object.

Render a text into a texture.
