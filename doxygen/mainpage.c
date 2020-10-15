/*
 * mainpage.c -- describe "Main Page" in Doxygen
 *
 * Copyright (c) 2020 David Demelier <markand@malikania.fr>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * \mainpage
 *
 * Welcome to the Molko's reference API.
 *
 * # About
 *
 * Molko's Adventure is a 2D solo RPG written in pure C, using SDL2 and SDL2
 * addons.
 *
 * As its heart, the game is split into two parts, the core (**src/core**) and
 * the game data and scenario (**src/adventure**). It is then possible to anyone
 * to create its own similar game using the same core. However please note that:
 *
 * - It is not a game engine! Many aspects in the core are still tightly coupled
 *   with the original game design. We wanted the core to be kept simple without
 *   thousands of dynamic allocations and genericity all over the place.
 * - It is not meant to be used as system library. Functions were kept simple
 *   and not prefixed by any "namespace"-like word. Instead, it is meant to be
 *   bundled with your game as boilerplate code with the possibility for you to
 *   change all internals if you want.
 *
 * # Usage
 *
 * If you plan to create your own game, simply copy the whole **src/core**
 * directory to your project and add all .c files to the compilation process. No
 * prior configuration is required, every features are detected using `#ifdef`s.
 *
 * \note If you find a bug regarding your platform, feel free to send bug
 *       reports and patches.
 *
 * Then, check this API or read .h files for documentation. The files ending
 * with _p.h are usually reserved for the implementation and should not be used
 * unless you know what you're doing.
 *
 * # Documentation nomenclature
 *
 * Every modules are organized per files and referenced this way. For example,
 * if you need to access the error handling, you just have to use \ref error.h
 * file.
 *
 * ## Structures
 *
 * Almost every structure are exposed directly to the user and allocated on the
 * stack. Each member is documented with the following prefix format (XYZ)
 * where:
 *
 * The letter *X* defines the following restriction
 *
 * - `+`: The property is readable/editable by the user,
 * - `-`: The property is readable by the user,
 * - `*`: The property is not meant to be used directly by the user.
 *
 * The letter *Y* can be set to `&` in that case means it is only referenced
 * and is not an owned property (usually a non-owning pointer). Which means
 * user is responsible for deallocation if required.
 *
 * The finall letter *Z* can be set to `?` which means it is optional (like a
 * nullable pointer).
 *
 * Examples:
 *
 * ```c
 * struct foo {
 *     int x;                // (+) Position in x.
 *     int y;                // (+) Position in y.
 *     struct theme *th;     // (+&?) Theme to use.
 *     unsigned int elapsed; // (-) Elapsed time since last frame.
 *     struct texture text;  // (*) Texture used for rendering.
 * };
 * ```
 *
 * Within this structure, the fields `x` and `y` are completely accessible to
 * the user for both reading/writing. The field `th` is an optional non-owning
 * pointer to a theme which is also readable/writable. The field `elapsed` is
 * readable but should not be modified. Finally, the field `text` is private
 * and should not be touched by the user.
 *
 * ## Functions
 *
 * When functions needs to initialize and destroy objects, the following
 * conventions is used:
 *
 * - `foo_init`: initialize the object,
 * - `foo_finish`: dispose the object and possible owned allocations.
 *
 * In case of opaque objects, the creation function is named under the module
 * discretion (e.g. `foo_open`, `foo_new`) but the destruction function is
 * usually named `foo_free`.
 *
 * # Usage conventions
 *
 * This chapter describes lots of aspects about using this core API.
 *
 * ## Memory and ownership
 *
 * Within the core, functions usually never take ownership of user objects. They
 * mostly assume that objects are still alive during usage of the core API.  If
 * the opposite case happen, it is clearly stated in the documentation. Unless
 * stated, user must always make sure objects are kept alive.
 *
 * If a function needs its own copy of something, it usually take a const
 * pointer and copy it into its internals. But make sure that every member are
 * still pointer to valid memory if this is a concern.
 *
 * Example: the \ref script.h modules is used to create a sequence of action, on
 * its internal it stores a singly-linked list of \ref action that are copied
 * from the user. But since the \ref action structure contains arbitrary data,
 * those must be kept until the action is no longer used.
 *
 * \code
 * struct action my_action action = {
 * 	.data = my_action_data,
 * 	.update = my_action_update
 * };
 * struct script script;
 *
 * script_init(&script);
 * script_append(&script, &action);
 * \endcode
 *
 * Even though \ref script_append copy the action into an internal object,
 * the *my_action_data* must still be valid until the script ended.
 *
 * ## Modules
 *
 * Whenever possible, functions that needs a structure context will always take
 * it as first argument.
 *
 * This let the user to control lifetime and allocation storage for most of the
 * API objects.
 *
 * Example:
 *
 * \code
 * struct clock clock;
 *
 * clock_start(&clock);
 * clock_elapsed(&clock);
 * \endcode
 *
 * ## Thread safety and reentrancy
 *
 * The core API is **not** thread-safe at all. Any module must always be used in
 * a single thread unless you really know what you're doing.
 *
 * Most of the API is reentrant though except the some modules which use global
 * objects.
 *
 * \note This may change in the future.
 *
 * ## Error handling
 *
 * Functions that may fail usually return a boolean. If an error occured, it is
 * stored in a global variable that are accessed through the \ref error.h
 * module.
 */
