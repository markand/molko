# molko_build_maps

Generate .map files from tiled .json files using the mlk-map tool. The file
hierarchy is kept and only extension is changed from .json to .map.

## Synopsis

```cmake
molko_build_maps(input outputs)
```

Argument outputs will be set with the generated output files in the binary
directory to be added as target sources.

Make sure to add `${CMAKE_CURRENT_BINARY_DIR}` into the target include
directories.

