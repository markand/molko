# Error handling

How error handling is used within the API and the user code.

## Synopsis

Error handling is always a complicated task in the software development. In
Molko's Adventure there is three kinds of errors:

1. Programming error.
2. Recoverable error at runtime.
3. Unexpected error at runtime.

## Kind of errors

### Assertions

One of the easiest errors to detect are about programming errors. In the
Molko's Adventure API they are usually detected at runtime when you use a
function with preconditions unmet. In that case standard C `assert` is used.

For example, it happens when you:

- Pass a NULL pointer where the function expects non-NULL,
- Pass invalid arguments such as out of bounds indicies.

Always read carefully preconditions that are annotated through the
documentation.

### Recoverable errors

Next, come recoverable errors. Those arise when you may expect a failure from
the API and can do something else.

For example, it happens when you:

- Open a font file that is invalid,
- Open a music file that is no longer on the disk.

In these situations, the API indicates usually by a return code that the
function was unable to complete correctly (and you can use error to retrieve
the specified error). In some case, you *MUST* not ignore the return value
because if the function takes an input as argument it will be kept untouched.
For example, the texture_new function may fail to create a texture and in that
case it is left untouched.

### Unexpected errors

Finally, come what we call unexpected errors. Those happen while they are not
supposed to.

For example, it happens when there is a severe issue either in the kernel,
graphics or any other library that aren't raised by the API itself.

This includes (but not limited to):

- A rendering error (caused by an internal issue).

In these situations, the API calls the function panic which definitely calls
the panic_handler. The default implementation prints an error and exit with a
code of 1. User may pass a custom function but should quit the game because the
API does not take care of deallocating data before calling panic.

## Error handling in Molko's Adventure API

The following table shows what is used and when.

|             | `assert`           | Booleans            | panic                                     | Notes                             |
|-------------|--------------------|---------------------|-------------------------------------------|-----------------------------------|
| libmlk-core | Programming errors | As much as possible | Only in memory utilities from util.h      | Never called from libcore itself. |
| libmlk-ui   | Programming errors | When applicable     | Mostly in rendering errors                | None.                             |
| libmlk-rpg  | Programming errors | When applicable     | Mostly in rendering errors                | None.                             |
