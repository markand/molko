# molko_build_tilesets

Generate .tileset files from tiled .json files using the mlk-tileset tool.
The file hierarchy is kept and only extension is changed from .json to
.tileset.

## Synopsis

```cmake
molko_build_tilesets(input outputs)
```

Argument outputs will be set with the generated output files in the binary
directory to be added as target sources.

Make sure to add `${CMAKE_CURRENT_BINARY_DIR}` into the target include
directories.

