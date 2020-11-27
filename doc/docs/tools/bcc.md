# Tool: mlk-bcc

This utility generates C byte array from any input file. Its main purpose is to
allow embedding data directly into the executable without having to load files
from disk.

It reads the given file and generate the output to the standard output.

Synopsis:

	mlk-bcc [-0csu] [-I num] [-i num] filename variable

Options and arguments:

-0
:   Terminate the array with a NUL terminator.

-c
:   Generate a ``const`` C array.

-s
:   Generate a ``static`` C array.

-u
:   Use ``unsigned char`` instead of ``signed char`` when generating the byte
    array.

-I num
:   Use ``num`` tabs to indent (defaults to 1).

-i num
:   Use ``num`` spaces to indent.

filename
:   Filename to read as input, a value of ``-`` will read the standard input.

variable
:   The C variable to generate, any character that is not valid in C will be
    replaced by a ``_`` but the variable must still not start with a digit.
