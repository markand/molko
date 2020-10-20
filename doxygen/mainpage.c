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
 * As its heart, the game is split into multiple parts, libraries, game data and
 * game code.
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
 * If you plan to create your own game, simply copy the whole directory
 * directory to your project and select which libraries you need.
 *
 * \note If you find a bug regarding your platform, feel free to send bug
 *       reports and patches.
 *
 * Then, check this API or read .h files for documentation. The files ending
 * with _p.h are usually reserved for the implementation and should not be used
 * unless you know what you're doing.
 *
 * # Libraries
 *
 * The following libraries are available and built separately:
 *
 * | Library | Description                                  | Dependencies                           |
 * |---------|----------------------------------------------|----------------------------------------|
 * | libcore | Low level audio, video, windowing and input. | SDL2, SDL2_mixer, SDL2_ttf, SDL2_image |
 * | libui   | Minimalist UI routines to draw interfaces    | libcore                                |
 * | librpg  | Group of modules related to RPG games        | libcore, libui                         |
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
 */
