Molko's Engine
==============

The Molko's Engine is a C and Javascript framework to write 2D RPG games in a
portable manner. It is designed in mind to be flexible, simple and convenient to
use.

It has the following features:

- Written in C99,
- Portable (Windows, macOS and UNIX likes systems),
- Simple, flexible,
- Well documented,
- Virtual file system support.
- Usable in both C or Javascript.
- Can embed resources using `mlk-bcc`.

Javascript quick startup
------------------------

The `mlk-run` executable exposes the Javascript API and start a game from a
directory or a .mlk file (which is a .zip file with a .mlk extension).

Hello world in Javascript:

	mkdir superluigi

Add the following content in superluigi/main.js:

```js
var w = new Window("Super Luigi, number one", 640, 480);
var run = true;

while (run) {
	var ev;

	while ((ev = Event.poll())) {
		switch (ev.type) {
		case Event.Type.QUIT:
			run = false;
			break;
		default:
			break;
		}

		Painter.color = 0xFFFFFFFF;
		Painter.clear();
		Painter.present();
	}
}
```

Finally run the game using `mlk-run superluigi`.

You can also create a .zip file with a .mlk extension and `mlk-run` will be able
to use content from it.

	cd superluigi
	zip -r ../superluigi.mlk .
	cd ..
	mlk-run superluigi.mlk

Documentation
-------------

Use `mkdocs serve` inside the **doc** directory to see the user and developer
documentation.

Author
------

Molko's Engine was written by David Demelier <markand@malikania.fr>.
