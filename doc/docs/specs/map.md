# Spec: map

Maps are probably the most important files within the game, it provides the map
definition, structure, actions and player interactions.

It consists of:

- A tileset to use (only one),
- A size in terms of number of rows and columns,
- 3 layers (background, foreground and above),
- 1 layer of actions.

Just like [tileset](tileset.md), maps are defined in a custom map file format in
plain text.

Maps have an extension of ``.map``.

## File format

The file format is a simple plain text where each each line consist of a
directive.

!!! caution
    Order is important. You must make sure that every directives are listed in
    the same order.

The file format must be defined as following where each ``<>`` must be replaced
by the appropriate C type name.

    title|<const char *>
    columns|<unsigned int>
    rows|<unsigned int>
    tileset|<const char *>
    origin|<int>|<int>
    layer|background
    <int>
    <int>
    ...
    layer|foreground
    <int>
    <int>
    ...
    layer|above
    <int>
    <int> 
    ...
    layer|actions
    <int>|<int>|<unsigned int>|<unsigned int>|<int>|<const char *>

Description of directives:

title
:   Map title, must not exceed 64 bytes.

columns
:   Number of columns.

rows
:   Number of rows.

tileset
:   The tileset to use given as a string name. The path must be relative.

origin
:   (Optional) Position in where the player starts bt specifying ``x``, ``y``
    respectively.

layer
:   (Optional) For layers of type ``background``, ``foreground`` and ``above``,
    defines the tiles to use in order from top-left to bottom-right starting
    from 1, a value of 0 means no tile to render. If a map has a dimensions of
    4 columns and 6 the number of tiles must not exceed 24.

    For the layer action, defines an object to be implemented in the API. The
    first 4 integers define the `x`, `y`, `width`, `height` dimensions of the
    action rectangle respectively. The fifth integer argument should be set to 1
    if it must collide with the player or 0 otherwise. The final string is an
    argument to pass to the action and may be omitted.

## Examples

Some examples of valid maps.

### Example of basic map

The following file is a map with 4 columns and 2 rows (it is probably small for
a real game but at least it is readable for this example). The player starts at
10, 10, the background is using the same tile while the foreground is using
different tiles.

    title|Small map
    columns|4
    rows|2
    origin|10|10
    layer|background
    1
    1
    1
    1
    1
    1
    1
    1
    layer|foreground
    0
    0
    2
    2
    0
    0
    3
    3

### Example of actions

!!! note
    For simplicity, we only focus on the `layer|actions` and assume that the
    beginning of the map is valid.

This layer of actions contains two blocking regions and one non-blocking action
that is named "game-over".

    layer|actions
    100|100|50|50|1
    200|200|50|50|1
    1680|2855|48|48|0|game-over

