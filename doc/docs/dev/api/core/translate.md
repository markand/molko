# Module: translate

Synopsis

```c
#include <core/translate.h>
```

Translation support.

!!! tip
    This module is usually not required in user code because translations are loaded
    from library themselves.

## Functions

### translate\_init

Initialize native language support for the given domain `name`. Returns false on
errors.

```c
bool
translate_init(const char *name)
```

### translate\_finish

Close translations.

```c
void
translate_finish(void)
```
