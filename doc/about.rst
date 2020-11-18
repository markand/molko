=====
About
=====

Molko's Adventure is a 2D solo RPG game and an API to build more RPG games. It
is designed with the following characteristics:

- Turn based battle. Similar to old RPG games battle operate on a dedicated game
  state where the player and enemies select actions to perform when the member
  is ready to play.
- Tile based maps using scrolling and pixel granularity movements.
- Theme system to allow customization of some user interface components.
- Set of predefined dialog windows to control the game.

While the API is flexible and powerful enough to create different games, it is
still kept as simple possible without cluttering the API with dozen of
abstraction and generics all over the place. This means that there are
predefined kinds of character status (poison, slow, etc) along with kind of
spells (earth, neutral).
