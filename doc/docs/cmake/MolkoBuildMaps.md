# molko_build_maps

Convert [Tiled][] maps and tilesets into textual representations that can be
loaded from Molko's Adventure API.

## Synopsis

```cmake
molko_build_maps(
OUTPUT_DIR  output directory
OUTPUTS     output variables
MAPS        (Optional) List of maps
TILESETS    (Optional) List of tilesets
)
```

Argument MAPS and TILESETS should contain list of .json files generated from
[Tiled][tiled] and store the result in OUTPUT_DIR

Arguments OUTPUTS will be filled with genereted files from CMake and can be
used as executable and input.

[Tiled]: http://mapeditor.org

