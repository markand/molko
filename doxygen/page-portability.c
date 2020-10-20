/**
 * \page portability Portability
 * \tableofcontents
 *
 * A note about portability in Molko's Adventure API.
 *
 * # C standard used
 *
 * The code is based mostly written in C99 with a subset of C11. The following
 * features from C11 are required:
 *
 * - `<stdnoreturn.h>` header.
 *
 * # POSIX
 *
 * A few routines from the POSIX specification are also used:
 *
 * - `<sys/stat.h>` header.
 * - `read(2)` function.
 * - `open(2)` function.
 * - `getopt(3)`,
 * - `fmemopen(3)`.
 *
 * Efforts are ongoing to provide fallback implementations for systems that lack
 * those functionalities.
 *
 * # System tested
 *
 * - macOS (10.15 and later),
 * - Linux (glibc and musl variants),
 * - FreeBSD, NetBSD and OpenBSD.
 *
 * Windows is currently not supported right now but will be in the future.
 */
