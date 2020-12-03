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
ASSETS              (Optional) list of assets
LIBRARIES           (Optional) libraries to link
PRIVATE_FLAGS       (Optional) C flags (without -D)
PRIVATE_INCLUDES    (Optional) local includes for the target only
PUBLIC_FLAGS        (Optional) C flags (without -D)
PUBLIC_INCLUDES     (Optional) includes to share with target dependencies
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

Optional argument *PRIVATE_FLAGS*, *PUBLIC_FLAGS*, *PRIVATE_INCLUDES*,
*PUBLIC_INCLUDES*, *LIBRARIES* may be passed to set compile flags, includes
and libraries respectively.

The arguments *ASSETS* contains a list of assets to be converted during the
build. The file hierarchy is conserved in the `${CMAKE_CURRENT_BINARY_DIR}`.

If *FOLDER* option is set, it is organized into its name under the IDE if
supported.

