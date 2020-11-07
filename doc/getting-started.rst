Getting started
===============

Before developing your game in Javascript, make sure you've installed Molko's
engine with Javascript support. This will provide the command line tool
``molko-js`` which is the default loader.

molko-js
--------

This executable loads the Javascript API and performs some modifications of the
core API such as:

- A panic handler is setup by default to show an error message if a programming
  or runtime error is detected at runtime. It will simply print the message on
  the screen and wait the user for action to quit.
- It loads automatically the core, UI and RPG API at startup.

The executable take only one argument, the path to the script file to run.

Example of use

.. code-block:: bash

  $ molko-js script.js
