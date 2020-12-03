# molko_build_assets

Generate custom commands to convert assets into C header files using mlk-bcc.
The argument output will be set to contain every generated output so that the
caller can set them as source input and be generated before the target itself.

## Synopsis:

```cmake
molko_build_assets(assets outputs)
```

## Example

```cmake
molko_build_assets(image.png outputs)
add_executable(main main.c ${outputs})
```

Do not forget to add the CMake current binary directory
`${CMAKE_CURRENT_BINARY_DIR}` through the include flags of the given target.

Each file is generated using the exact same file hierarchy as the input so
an input of foo/bar/baz.png will be generated as foo/bar/baz.h in the binary
directory. The exported symbol use the pattern <last-directory>_<basename> so
in the above example, the file variable would be "bar_baz"

