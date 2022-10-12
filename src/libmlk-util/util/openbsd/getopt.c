/*
 * Copyright (c) 1987, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	util_opterr = 1,		/* if error message should be printed */
	util_optind = 1,		/* index into parent argv vector */
	util_optopt,			/* character checked for validity */
	util_optreset;		/* reset getopt */
char	*util_optarg;		/* argument associated with option */

#define	BADCH	(int)'?'
#define	BADARG	(int)':'
#define	EMSG	""

/*
 * getopt --
 *	Parse argc/argv argument vector.
 */
int
util_getopt(int nargc, char * const *nargv, const char *ostr)
{
	static char *place = EMSG;		/* option letter processing */
	char *oli;				/* option letter list index */

	if (ostr == NULL)
		return (-1);

	if (util_optreset || !*place) {		/* update scanning pointer */
		util_optreset = 0;
		if (util_optind >= nargc || *(place = nargv[util_optind]) != '-') {
			place = EMSG;
			return (-1);
		}
		if (place[1] && *++place == '-') {	/* found "--" */
			++util_optind;
			place = EMSG;
			return (-1);
		}
	}					/* option letter okay? */
	if ((util_optopt = (int)*place++) == (int)':' ||
	    !(oli = strchr(ostr, util_optopt))) {
		/*
		 * if the user didn't specify '-' as an option,
		 * assume it means -1.
		 */
		if (util_optopt == (int)'-')
			return (-1);
		if (!*place)
			++util_optind;
		if (util_opterr && *ostr != ':')
			(void)fprintf(stderr,
			    "illegal option -- %c\n", util_optopt);
		return (BADCH);
	}
	if (*++oli != ':') {			/* don't need argument */
		util_optarg = NULL;
		if (!*place)
			++util_optind;
	}
	else {					/* need an argument */
		if (*place)			/* no white space */
			util_optarg = place;
		else if (nargc <= ++util_optind) {	/* no arg */
			place = EMSG;
			if (*ostr == ':')
				return (BADARG);
			if (util_opterr)
				(void)fprintf(stderr,
				    "option requires an argument -- %c\n",
				    util_optopt);
			return (BADCH);
		}
		else				/* white space */
			util_optarg = nargv[util_optind];
		place = EMSG;
		++util_optind;
	}
	return (util_optopt);			/* dump back option letter */
}
