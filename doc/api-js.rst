.. toctree::
   :hidden:

   api-js-starter

.. highlight:: js

==============
Javascript API
==============

The Javascript API is designed to allow developing RPG games in a more
convenient, quicker and more flexible way. By using the Javascript API you may
deploy your game without having to compile the project for every platform.


Conformance
-----------

The Javascript engine is powered by Duktape_, it supports ECMAScript 5.1 with
some extensions of newer specifications. See the official `features tables`_ for
more information.

Modules support
---------------

Support of CommonJS modules is available with some limitations. It only loads
Javascript files and the root path is taken from the initial script startup
file.

In other terms, given the following hierarchy::

  -- mygame/
  --   main.js
  --   actions/
  --     chest.js
  --     util.js

In any file of the given hierarchy, calling ``require("foo")`` will use the
parent *mygame* directory as origin. Otherwise, if a call to ``require`` starts
with *./* then it is relative to the current file being loaded. As such, in
*chest.js* you would typically use ``require("./util")`` to load *util.js* as
it is lives in the same directory as *chest.js*.

On the other hand, in the *main.js* file you may invoke
``require("./actions/chest")`` or ``require("actions/util");`` as both will work
in that case.

.. note:: Do not append *.js* extension in any require call, it is added
          automatically from the code.

Example of usage *main.js*

::

  var chest = require("./actions/chest");
  chest.create();

And in *actions/chest.js*

::

  var util = require("./util");
  
  exports.create = function () {
    print("Super cool exported function");
  };

.. tip:: We recommend that you use local requires using *./* for files that are
         in the same directory or in a descendant directory and use "absolute"
         paths when referring to a sibling or ancestor directory.

Javascript modules
------------------

List of all available modules.

.. toctree::
   :maxdepth: 1

   api-molko-clock
   api-molko-event
   api-molko-font
   api-molko-music
   api-molko-painter
   api-molko-sprite
   api-molko-texture
   api-molko-util
   api-molko-window

.. _Duktape: http://duktape.org
.. _features tables: https://wiki.duktape.org/postes5features
