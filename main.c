/*
 * Copyright (c) 2012 Rafael F. Zalamena <rzalamena@gmail.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
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

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <lib/putil.h>
#include <lib/pastebin.h>

#define VERSION_STR	"0.1"

extern char	*__progname;

static __dead void
usage(void)
{
	fprintf(stderr,
	    "Usage:\n%1$s <-f file_name> <-t type>\n"
	    "%1$s -v\n",
	    __progname);
	exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
	FILE			*input_fd;
	size_t			 read_ret;
	char			 copt, filename[FILENAME_MAX];
	char			 buf[524288];
	struct paste_ctx	*pctx;

	pctx = new_pb_paste();
	input_fd = NULL;

	if (argc == 1)
		usage();

	while ((copt = getopt(argc, argv, "vf:i:n:")) != -1) {
		switch (copt) {
		case 'v':
			printf("%s current version: %s\n",
			    __progname, VERSION_STR);
			exit(EXIT_SUCCESS);
			break;

		case 'f':
			if (strlcpy(filename, optarg, (sizeof(filename) - 1))
			    >= (sizeof(filename) - 1)) {
				fprintf(stderr, "user input error: "
				    "input filename is too long.\n");
				exit(EXIT_SUCCESS);
			}
			input_fd = fopen(filename, "r");
			break;

		case 'i':
			pb_paste_format_id(pctx, strtol(optarg, NULL, 0));
			break;

		case 'n':
			pb_paste_format_name(pctx, optarg);
			break;
		}
	}

	if (input_fd == NULL)
		err(1, "open()");
	read_ret = fread(buf, sizeof(char), (sizeof(buf) - 1), input_fd);
	if (read_ret == 0) {
		fprintf(stderr, "fread() error.\n");
		exit(EXIT_FAILURE);
	}

	fprintf(stderr, "[%s]\n", putil_send(pctx, buf));

	exit(EXIT_SUCCESS);
}
