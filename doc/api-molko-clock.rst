.. toctree::
.. _api-molko-clock:

Molko.Clock
===========

Keep track of elapsed time.

Constructors
------------

.. js:function:: Molko.Clock()

Create a new clock object and start collecting time from now. This function
MUST be called as a constructor, not doing so will also raise an error.

Methods
-------

.. js:method:: Molko.Clock.start()

Reset the clock from now.

Properties
----------

The following properties are available once an object has been constructed:

.. js:attribute:: Molko.Clock.elapsed

(Read-only) Number of milliseconds elapsed since construction of object or last
call to start function.
