#
# CMakeLists.txt -- CMake build system for Molko's Engine
#
# Copyright (c) 2020-2023 David Demelier <markand@malikania.fr>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#

mlk_option(MLK_WITH_DOXYGEN On
    BOOL "Enable doxygen documentation")
mlk_option(MLK_WITH_EXAMPLES On
    BOOL "Enable examples")
mlk_option(MLK_WITH_NLS On
    BOOL "Enable NLS support")
mlk_option(MLK_WITH_TESTS On
    BOOL "Enable unit tests")
mlk_option(MLK_WITH_TESTS_GRAPHICAL On
    BOOL "Enable unit tests that requires graphical context")
mlk_option(MLK_WITH_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake"
    STRING "Destination for CMake files")
