.. toctree::
.. _api-molko-texture:

=============
Molko.Texture
=============

This module provides graphics primites for textures. Textures can be created by
hand or loaded from files. It's possible to draw on a texture using the
:ref:`api-molko-painter` API.

Constants
---------

.. js:data:: Molko.Texture.BlendMode

This enumeration constant contains the following blend mode that can be used as
the ``blendMode`` property.

``TEXTURE_BLEND_NONE``
  No modification.

``TEXTURE_BLEND_BLEND``
  Alpha blend mode modulation.

``TEXTURE_BLEND_ADD``
  Additive modulation.

``TEXTURE_BLEND_MODULATE``
  Color modulation.

Constructors
------------

.. js:function:: Molko.Texture(width, height)

   :param uint width: Desired texture width.
   :param uint height: Desired texture height.

Construct a new texture with the given dimensions. This function MUST be called
as a constructor, not doing so will also raise an error.

Functions
---------

.. js:function:: Molko.Texture.fromImage(path)

   :param string path: Path to the image file.
   :throws: Error in case of error.
   :returns: A Texture object.

Try to open an image as a texture.

Methods
-------

.. js:method:: Molko.Texture.draw(x, y)

   :param int x: Position in x.
   :param int y: Position in y.
   :throws: Error on errors.

Draw the whole texture at the given position.

.. js:method:: Molko.Texture.scale(source, destination, angle = 0)

   :param object source: Source rectangle.
   :param object destination: Destination rectangle.
   :param number angle: Angle rotation.
   :throws: Error on errors.

Scale the texture from the source region into the destination and apply an
optional angle.

The ``source`` and ``destination`` objects MUST contain the following
properties:

``x (int)``
  Start position in x.
``y (int)``
  Start position in y.
``h (int)``
  Height to scale.
``w (int)``
  Width to scale.

Properties
----------

.. js:attribute:: Molko.Texture.width

(Read-only) Texture's width.

.. js:attribute:: Molko.Texture.height

(Read-only) Texture's height.

.. js:attribute:: Molko.Texture.blendMode

(Write-only) Change the blend mode.

.. js:attribute:: Molko.Texture.alphaMod

(Write-only) Change the alpha transparency modulation from 0 to 255.

.. js:attribute:: Molko.Texture.colorMod

(Write-only) Change color modulation.
