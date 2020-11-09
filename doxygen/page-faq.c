/**
 * \page faq FAQ
 * \tableofcontents
 *
 * # Why not creating a game engine?
 *
 * Game engines are usually too generic, too complex and do too much things.
 * Lots of them also use scripting language which require another learning curve
 * to be used.
 *
 * Molko's Adventure is a solo tactical 2D RPG and its core is especially
 * designed for that gameplay. Thus the code stay simple to understand and to
 * follow. Coupling a general purpose game engine with a game design is
 * sometimes more complicated than writing small code.
 *
 * However, game engines are still interesting for people who already know how
 * to use them and feel confident with them.
 *
 * # Why no prefix like "ma_" or "MA_"?
 *
 * See also question above.
 *
 * The purpose of Molko's Adventure is not to provide a drop-in reusable library
 * to create RPG games. It is highly coupled with the gameplay of the original
 * game. Thus we wanted to keep the codebase extremely simple and minimalist
 * without adding bunch of genericity.
 *
 * The core API isn't meant to be installed system wide (possible, but not
 * recommended), instead users are encouraged to copy the core code and to adapt
 * to their gameplay.
 *
 * A simple example: if someone wants to create a game and would like to allow
 * only three playable entities at a time, it should simply edit appropriates
 * structures and everything is adapted. Then, structures and array can stay
 * static without dynamic allocations.
 *
 * # Why C instead of <FOO>?
 *
 * C is an awesome language. It still has its place in the industry especially
 * in low-level, kernel and game design.
 *
 * Games are usually designed without OO in mind using simple procedural codes
 * and lots of data. In C, writing code makes easier to understand what's
 * happening under the hood without having to check if functions will generate
 * dynamic allocations or not.
 *
 * However, we also love modernity and as such, C11 is the minimal requirement
 * to build and run Molko's Adventure.
 *
 * \note The code does not make any assumption about the platform it runs, it
 *       expects to have a fully conformant C11 implementation. Microsoft MSVC
 *       is known to **not** support this C version.
 * 
 * # Can I make a MMORPG with that?
 *
 * Not easily.
 *
 * This core API is really tied to a unique solo RPG adventure and therefore it
 * does not separate logic from rendering.
 *
 * Also, note that creating a server-client game is really different in terms of
 * architecture than a local solo game. Most of the logic part is done server
 * side and requires much more code to analyze to avoid cheats, lag,
 * synchronisation and many other stuff than a local game does not require.
 *
 * There are no plans to create a network oriented core API anytime soon.
 */
