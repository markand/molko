/**
 * \page howto-init Howto: initialization
 * \tableofcontents
 *
 * Howto initialize the core.
 *
 * \section synopsis Synopsis
 *
 * Before using the core, you need to initialize the subsystems and internal
 * backend API.
 *
 * \warning Even non-rendering modules requires initialization and some of them
 *          even require a window to be open.
 *
 * \section usage Usage
 *
 * The following table summarize the functions to be used at the beginning and
 * the end of your game.
 *
 * | System  | Init function    | Close function    | Remarks                |
 * |---------|------------------|-------------------|------------------------|
 * | General | \ref sys_init    | \ref sys_close    | Required for most API  |
 * | Window  | \ref window_init | \ref window_close | Required by some parts |
 *
 * All init functions set an error code if any and you're encouraged to test the
 * result and check the error if any.
 *
 * \section example Example
 *
 * Init the core and create a window of Full HD resolution. The function \ref
 * error_fatal will print the error and exit if needed.
 *
 * \code
 * if (!sys_init())
 * 	error_fatal();
 * if (!window_init("My Awesome Game", 1920, 1080))
 * 	error_fatal();
 * \endcode
 */