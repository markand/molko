# Module: save

Synopsis

```c
#include <core/save.h>
```

Save functions.

This module provides several functions to save the game data into a database.

Database can be opened in read only mode `SAVE_MODE_READ` which will return an
error if not present or write mode `SAVE_MODE_WRITE` which will create and
initialize a database if not present on disk.

## Macros

### SAVE\_PROPERTY\_KEY\_MAX

Maximum property key length.

```c
#define SAVE_PROPERTY_KEY_MAX (64)
```

### SAVE\_PROPERTY\_VALUE\_MAX

Maximum property value length.

```c
#define SAVE_PROPERTY_VALUE_MAX (1024)
```

## Enums

### save\_mode

Open mode for loading database.

| Enumerator        | Description                        |
|-------------------|------------------------------------|
| `SAVE_MODE_READ`  | Try to read (no creation).         |
| `SAVE_MODE_WRITE` | Open for both reading and writing. |

## Structs

### save

Database handle.

| Field                       | Access | Type     |
|-----------------------------|--------|----------|
| [created](#created-updated) | (-)    | `time_t` |
| [updated](#created-updated) | (-)    | `time_t` |

#### created, updated

Timestamp when the save was created and last modified respectively.

### save\_property

Structure that describe a generic key-value property.

| Field                 | Access | Type                                      |
|-----------------------|--------|-------------------------------------------|
| [key](#key-value)     | (+)    | `char key[SAVE_PROPERTY_KEY_MAX + 1]`     |
| [value](#key-value)   | (+)    | `char value[SAVE_PROPERTY_VALUE_MAX + 1]` |

#### key, value

Property key and value.

## Functions

### save\_open

Open and load the database `db` numbered `idx`. The argument `mode` controls the
opening mode.

This function use the preferred path to store local files under the user home
directory. The parameter idx specifies the save slot to use.

Returns false on errors, in this case `db` remains uninitialized and must not be
used.

```c
bool
save_open(struct save *db, unsigned int idx, enum save_mode mode)
```

### save\_open\_path

Similar to [save_open](#save_open) but use a `path` instead.

```c
bool
save_open_path(struct save *db, const char *path, enum save_mode mode)
```

### save\_set\_property

Sets the property `prop` in the database `db`.

If the property already exists, replace it.

Returns false on errors.

```c
bool
save_set_property(struct save *db, const struct save_property *prop)
```

### save\_get\_property

Get the property value from the database `db` and store result in `prop`.
Returns false on errors.

```c
bool
save_get_property(struct save *db, struct save_property *prop)
```

### save\_remove\_property

Remove the property `prop` from the database `db`. Returns false on errors.

```c
bool
save_remove_property(struct save *db, const struct save_property *prop)
```

### save\_finish

Close the database `db`.

```c
void
save_finish(struct save *db)
```
