# molko_define_executable

Create an executable.

## Synopsis

```cmake
molko_define_test(
TARGET              target name
SOURCES             src1, src2, srcn
FOLDER              (Optional) IDE folder if supported
TRANSLATIONS        (Optional) list of translations
ASSETS              (Optional) list of assets to build
FLAGS               (Optional) C flags (without -D)
LIBRARIES           (Optional) libraries to link
INCLUDES            (Optional) includes
)
```

Create an executable with the name *TARGET* with the given *SOURCES*.

Optional include paths, libraries and flags can be specified via *INCLUDES*,
*LIBRARIES* and *FLAGS* arguments respectively.

If argument *ASSETS* is set, they are generated in the target binary
directory.

The optional argument *TRANSLATIONS* should contain a list of supported
translations. See molko_build_translations for more info.

