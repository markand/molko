# Howto: initialization

Howto initialize the API before developing.

## Synopsis

Before using the core, you need to initialize the subsystems and internal
backend API.

!!! caution
    Even non-rendering modules requires initialization and some of them even
    require a window to be open.

## Usage

The following table summarize the functions to be used at the beginning and
the end of your game.

| System  | Init function    | Close function     | Remarks                |
|---------|------------------|--------------------|------------------------|
| General | sys_init         | sys_finish         | Required for most API  |
| Window  | window_init      | window_finish      | Required by some parts |

All init functions set an error code if any and you're encouraged to test the
result and check the error if any.

## Example

Init the core and create a window of Full HD resolution. The function \ref
panic will all the panic handler.

```c
if (!sys_init())
	panic();
if (!window_init("My Awesome Game", 1920, 1080))
	panic();
```
