# molko_define_test

Create unit test.

## Synopsis

```cmake
molko_define_test(
TARGET      target name
SOURCES     src1, src2, srcn
ASSETS      (Optional) list of assets to build
FLAGS       (Optional) C flags (without -D)
LIBRARIES   (Optional) libraries to link
INCLUDES    (Optional) includes
)
```

Create an executable with the name *TARGET* and a test case of the same name
with the given *SOURCES*.

Optional include paths, libraries and flags can be specified via *INCLUDES*,
*LIBRARIES* and *FLAGS* arguments respectively.

If argument *ASSETS* is set, they are generated in the target binary
directory.

