# molko_define_library

Create any kind of library.

## Synopsis

```cmake
molko_define_library(
TARGET              target name
SOURCES             src1, src2, srcn
EXTERNAL            (Optional) set to true for external libraries
FOLDER              (Optional) optional subfolder to organize
TYPE                (Optional) type of library
LIBRARIES           (Optional) libraries to link
FLAGS               (Optional) C flags (without -D)
INCLUDES            (Optional) local includes for the target only
)
```

Create a library and optionally install it.

The function create a new library named with the parameter *TARGET*, you
should prefix it with "lib" as its the convention within molko (e.g. libfoo),
the prefix is automatically removed.

The argument *SOURCES* should contains the C source files and *HEADERS*
should points to a directory to be installed verbatim in the include
directory.

Optional argument *EXTERNAL* should be set for targets that are not
maintained here (e.g. third party libraries embedded).

The optional arguments *LIBRARIES*, *FLAGS* and *INCLUDES* are passed to the
respective CMake command [target_link_libraries][],
[target_compile_definitions][] and [target_include_directories][]
respectively. As such, it is necessary to specify the scope (PUBLIC, PRIVATE
or INTERFACE) for every argument.

If *FOLDER* option is set, it is organized into its name under the IDE if
supported.

[target_compile_definitions]: https://cmake.org/cmake/help/latest/command/target_compile_definitions.html
[target_include_directories]: https://cmake.org/cmake/help/latest/command/target_include_directories.html
[target_link_libraries]: https://cmake.org/cmake/help/latest/command/target_link_libraries.html

