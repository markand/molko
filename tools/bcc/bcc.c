/*
 * molko-bcc.c -- simple binary compiler
 *
 * Copyright (c) 2020 David Demelier <markand@malikania.fr>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#define _XOPEN_SOURCE 700
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <unistd.h>

static const char *charset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
static bool fstatic;
static bool nullterm;

noreturn static void
usage(void)
{
	fprintf(stderr, "usage: molko-bcc [-s] input varname\n");
	exit(1);
}

noreturn static void
die(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	fputs("abort: ", stderr);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	exit(1);
}

static char *
mangle(char *variable)
{
	char *p;
	size_t pos;

	/* Remove extension. */
	if ((p = strrchr(variable, '.')))
		*p = '\0';

	/* Remove disallowed characters. */
	while ((pos = strspn(variable, charset)) != strlen(variable))
		variable[pos] = '_';

	return variable;
}

static void
process(const char *input, const char *variable)
{
	FILE *fp;
	int ch, idx = 0;

	if (strcmp(input, "-") == 0)
		fp = stdin;
	else if (!(fp = fopen(input, "rb")))
		die("%s: %s\n", input, strerror(errno));

	if (fstatic)
		printf("static ");

	printf("const unsigned char %s[] = {\n", variable);

	while ((ch = fgetc(fp)) != EOF) {
		if (idx == 0)
			putchar('\t');

		printf("0x%02x, ", (unsigned char)ch);

		if (++idx == 4) {
			idx = 0;
			putchar('\n');
		}
	}

	/* Add final '\0' if requested. */
	if (nullterm) {
		if (idx++ == 0)
			putchar('\t');

		printf("0x00");
	}

	if (idx != 0)
		printf("\n");

	puts("};");
	fclose(fp);
}

int
main(int argc, char **argv)
{
	int ch;

	while ((ch = getopt(argc, argv, "0s")) != -1) {
		switch (ch) {
		case '0':
			nullterm = true;
			break;
		case 's':
			fstatic = true;
			break;
		default:
			break;
		}
	}

	argc -= optind;
	argv += optind;

	if (argc < 2)
		usage();

	process(argv[0], mangle(argv[1]));
}
