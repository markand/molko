.. toctree::
   :hidden:

Specifications
==============

This page describe file data, conventions and project structure required to
write a game using Molko's Adventure API. Most of the code relies on the
`conventions over configuration`_ principle where the user data is required to
be in a certain manner rather than allowing lots of parameters. In that way both
of the API and the user code stay solid and simple.

For example, the game expect sprites to be designed in such a way that the code
can simply select a row/column depending on the orientation. See individual
specifications for more details.

.. toctree::
   :maxdepth: 1

   spec-tileset
   spec-map

.. _conventions over configuration: https://en.wikipedia.org/wiki/Convention_over_configuration
