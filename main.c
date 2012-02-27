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
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <lib/putil.h>
#include <lib/pastebin.h>

#define VERSION_STR	"0.1"

extern const char	*__progname;

static __dead void
usage(void)
{
	fprintf(stderr,
	    "Usage:\n"
	    "'%1$s -p <-h>' - Use pastebin.com\n"
	    "'%1$s -v' - Show version\n"
	    "---\n"
	    "Additional parameters:\n"
	    "  -f <FILENAME> - Selects file to upload.\n"
	    "  -i <type_id> or -n <type_name> - Selects file type.\n"
	    "  -h - Show help and list options.\n"
	    "  -d - Paste duration.\n"
	    "  -y - Paste privacy setting.\n"
	    "  -m - Paste name.\n",
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
	int			 paste_dest, f_flag;
	long			 duration;
	struct paste_ctx	*pctx;

	pctx = new_pb_paste();
	input_fd = NULL;

	if (argc == 1)
		usage();

	f_flag = 0;
	paste_dest = 0;
	while ((copt = getopt(argc, argv, "hpvf:i:n:d:y:m:")) != -1) {
		switch (copt) {
		case 'p':
			/* FIXME use enumerators and add other paste
			 * destinations support.
			 */
			paste_dest = 1;
			break;

		case 'h':
			if (paste_dest == 1)
				pb_show_options();

			exit(EXIT_SUCCESS);
			break;

		case 'v':
			printf("%s current version: %s\n",
			    __progname, VERSION_STR);
			exit(EXIT_SUCCESS);
			break;

		case 'f':
			if (f_flag == 1)
				break;

			f_flag = 1;
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

		case 'd':
			duration = strtol(optarg, NULL, 0);
			if (duration == 0 &&
			    (errno == EINVAL || errno == ERANGE))
				duration = PB_TEN_MINUTES;

			/* XXX extra care here, since '0' means never
			 * expires
			 */
			pb_duration(pctx, duration);
			break;

		case 'y':
			pb_privacy(pctx, strtol(optarg, NULL, 0));
			break;

		case 'm':
			pb_paste_name(pctx, optarg);
			break;
		}
	}

	if (paste_dest == 0
	    || f_flag == 0)
		usage();

	if (input_fd == NULL)
		err(1, "open()");
	read_ret = fread(buf, sizeof(char), (sizeof(buf) - 1), input_fd);
	buf[sizeof(buf) - 1] = 0;
	if (read_ret == 0) {
		fprintf(stderr, "fread() error.\n");
		exit(EXIT_FAILURE);
	}

	printf("Your raw paste URL is:\n"
	    "%s\n", putil_send(pctx, buf));

	exit(EXIT_SUCCESS);
}
