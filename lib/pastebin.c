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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <lib/putil.h>
#include <lib/pastebin.h>
#include <lib/urlcode.h>

struct paste_format {
	const char	 name[64];
	const int	 key;
} paste_format_list[] = {
	{ "4cs",		  1 },
	{ "6502acme",		  2 },
	{ "6502kickass",	  3 },
	{ "6502tasm",		  4 },
	{ "abap",		  5 },
	{ "actionscript",	  6 },
	{ "actionscript3",	  7 },
	{ "ada",		  8 },
	{ "algol68",		  9 },
	{ "apache",		 10 },
	{ "applescript",	 11 },
	{ "apt_sources",	 12 },
	{ "asm",		 13 },
	{ "asp",		 14 },
	{ "autoconf",		 15 },
	{ "autohotkey",		 16 },
	{ "autoit",		 17 },
	{ "avisynth",		 18 },
	{ "awk",		 19 },
	{ "bascomavr",		 20 },
	{ "bash",		 21 },
	{ "basic4gl",		 22 },
	{ "bibtex",		 23 },
	{ "blitzbasic",		 24 },
	{ "bnf",		 25 },
	{ "boo",		 26 },
	{ "bf",			 27 },
	{ "c",			 28 },
	{ "c_mac",		 29 },
	{ "cil",		 30 },
	{ "csharp",		 31 },
	{ "cpp",		 32 },
	{ "cpp-qt",		 33 },
	{ "c_loadrunner",	 34 },
	{ "caddcl",		 35 },
	{ "cadlisp",		 36 },
	{ "cfdg",		 37 },
	{ "chaiscript",		 38 },
	{ "clojure",		 39 },
	{ "klonec",		 40 },
	{ "klonecpp",		 41 },
	{ "cmake",		 42 },
	{ "cobol",		 43 },
	{ "coffeescript",	 44 },
	{ "cfm",		 45 },
	{ "css",		 46 },
	{ "cuesheet",		 47 },
	{ "d",			 48 },
	{ "dcs",		 49 },
	{ "delphi",		 50 },
	{ "oxygene",		 51 },
	{ "diff",		 52 },
	{ "div",		 53 },
	{ "dos",		 54 },
	{ "dot",		 55 },
	{ "e",			 56 },
	{ "ecmascript",		 57 },
	{ "eiffel",		 58 },
	{ "email",		 59 },
	{ "epc",		 60 },
	{ "erlang",		 61 },
	{ "fsharp",		 62 },
	{ "falcon",		 63 },
	{ "fo",			 64 },
	{ "f1",			 65 },
	{ "fortran",		 66 },
	{ "freebasic",		 67 },
	{ "freeswitch",		 68 },
	{ "gambas",		 69 },
	{ "gml",		 70 },
	{ "gdb",		 71 },
	{ "genero",		 72 },
	{ "genie",		 73 },
	{ "gettext",		 74 },
	{ "go",			 75 },
	{ "groovy",		 76 },
	{ "gwbasic",		 77 },
	{ "haskell",		 78 },
	{ "hicest",		 79 },
	{ "hq9plus",		 80 },
	{ "html4strict",	 81 },
	{ "html5",		 82 },
	{ "icon",		 83 },
	{ "idl",		 84 },
	{ "ini",		 85 },
	{ "inno",		 86 },
	{ "intercal",		 87 },
	{ "io",			 88 },
	{ "j",			 89 },
	{ "java",		 90 },
	{ "java5",		 91 },
	{ "javascript",		 92 },
	{ "jquery",		 93 },
	{ "kixtart",		 94 },
	{ "latex",		 95 },
	{ "lb",			 96 },
	{ "lsl2",		 97 },
	{ "lisp",		 98 },
	{ "llvm",		 99 },
	{ "locobasic",		100 },
	{ "logtalk",		101 },
	{ "lolcode",		102 },
	{ "lotusformulas",	103 },
	{ "lotusscript",	104 },
	{ "lscript",		105 },
	{ "lua",		106 },
	{ "m68k",		107 },
	{ "magiksf",		108 },
	{ "make",		109 },
	{ "mapbasic",		110 },
	{ "matlab",		111 },
	{ "mirc",		112 },
	{ "mmix",		113 },
	{ "modula2",		114 },
	{ "modula3",		115 },
	{ "68000devpac",	116 },
	{ "mpasm",		117 },
	{ "mxml",		118 },
	{ "mysql",		119 },
	{ "newlisp",		120 },
	{ "text",		121 },
	{ "nsis",		122 },
	{ "oberon2",		123 },
	{ "objeck",		124 },
	{ "objc",		125 },
	{ "ocaml-brief",	126 },
	{ "ocaml",		127 },
	{ "pf",			128 },
	{ "glsl",		129 },
	{ "oobas",		130 },
	{ "oracle11",		131 },
	{ "oracle8",		132 },
	{ "oz",			133 },
	{ "pascal",		134 },
	{ "pawn",		135 },
	{ "pcre",		136 },
	{ "per",		137 },
	{ "perl",		138 },
	{ "perl6",		139 },
	{ "php",		140 },
	{ "php-brief",		141 },
	{ "pic16",		142 },
	{ "pike",		143 },
	{ "pixelbender",	144 },
	{ "plsql",		145 },
	{ "postgresql",		146 },
	{ "povray",		147 },
	{ "powershell",		148 },
	{ "powerbuilder",	149 },
	{ "proftpd",		150 },
	{ "progress",		151 },
	{ "prolog",		152 },
	{ "properties",		153 },
	{ "providex",		154 },
	{ "purebasic",		155 },
	{ "pycon",		156 },
	{ "python",		157 },
	{ "q",			158 },
	{ "qbasic",		159 },
	{ "rsplus",		160 },
	{ "rails",		161 },
	{ "rebol",		162 },
	{ "reg",		163 },
	{ "robots",		164 },
	{ "rpmspec",		165 },
	{ "ruby",		166 },
	{ "gnuplot",		167 },
	{ "sas",		168 },
	{ "scala",		169 },
	{ "scheme",		170 },
	{ "scilab",		171 },
	{ "sdlbasic",		172 },
	{ "smalltalk",		173 },
	{ "smarty",		174 },
	{ "sql",		175 },
	{ "systemverilog",	176 },
	{ "tsql",		177 },
	{ "tcl",		178 },
	{ "teraterm",		179 },
	{ "thinbasic",		180 },
	{ "typoscript",		181 },
	{ "unicon",		182 },
	{ "uscript",		183 },
	{ "vala",		184 },
	{ "vbnet",		185 },
	{ "verilog",		186 },
	{ "vhdl",		187 },
	{ "vim",		188 },
	{ "visualprolog",	189 },
	{ "vb",			190 },
	{ "visualfoxpro",	191 },
	{ "whitespace",		192 },
	{ "whois",		193 },
	{ "winbatch",		194 },
	{ "xbasic",		195 },
	{ "xml",		196 },
	{ "xorg_conf",		197 },
	{ "xpp",		198 },
	{ "yaml",		199 },
	{ "z80",		200 },
	{ "zxbasic",		201 },
	{ "",			  0 },
};

struct pastebin_ctx {
#ifdef __notyet__
	/* this is planned for logged users support */
	char			 user[MAXHOSTNAMELEN];
	char			 pass[128];
#endif

	enum pb_paste_duration	 duration;
	enum pb_privacy_setting	 privacy;
	struct paste_format	*type;
};

static char		*send_cb_pastebin(struct paste_ctx *, const u_char *);
static const char	*pb_conv_expire_date(enum pb_paste_duration);
static const char	*pb_conv_privacy(enum pb_privacy_setting);
static struct paste_format *pb_paste_format_by_name(const char *);
static struct paste_format *pb_paste_format_by_index(const int);

struct paste_ctx *
new_pb_paste(void)
{
	struct paste_ctx	*new;
	struct pastebin_ctx	*ctx;

	new = malloc(sizeof(struct paste_ctx));
	if (new == NULL)
		return (NULL);

	ctx = malloc(sizeof(struct pastebin_ctx));
	if (ctx == NULL) {
		free(new);
		return (NULL);
	}

	memset(new, 0, sizeof(struct paste_ctx));
	memset(ctx, 0, sizeof(struct pastebin_ctx));

	/* Turn that paste context into a pastebin ctx */
	new->data = ctx;
	new->send_cb = send_cb_pastebin;

	/* Set default sane settings */
	ctx->duration = PB_TEN_MINUTES;
	ctx->privacy = PB_UNLISTED;
	ctx->type = &paste_format_list[120]; /* defaults to 'text' */

	return (new);
}

static char *
send_cb_pastebin(struct paste_ctx *pctx, const u_char *buf)
{
	struct pastebin_ctx	*ctx;
	struct addrinfo		*res, *it;
	struct addrinfo		 hints;
	int			 sd, status;
	char			*buffer, *answer;
	char			*saux, *bufaux;
	char			 req_header[256], req_options[256];
	size_t			 hdr_len, req_len, total_len;
	size_t			 req_opt_len, real_req_len;
	ssize_t			 ret;

	ctx = pctx->data;

	/* Find out pastebin.com IP address */
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	status = getaddrinfo(PASTEBIN_HOST, "www", &hints, &res);
	if (status != 0) {
		fprintf(stderr, "getaddrinfo(): %s",
		    gai_strerror(status));
		return (NULL);
	}

	/* Iterate over all IP results possibilities */
	status = -1;
	for (it = res; it; it = it->ai_next) {		
		sd = socket(it->ai_family, it->ai_socktype,
		    it->ai_protocol);
		if (sd == -1) {
			warn("socket()");
			continue;
		}

		status = connect(sd, it->ai_addr,
		    res->ai_addrlen);
		if (status == -1) {
			warn("connect()");
			close(sd);
			continue;
		}
		status = 0;
		break;
	}
	freeaddrinfo(res);

	if (status != 0)
		return (NULL);

	/* Encode everything from user */
	saux = url_encode(buf);
	if (saux == NULL) {
		close(sd);
		return (NULL);
	}

	/* Generate HTTP post payload */
	req_len = strnlen(saux, PASTEBIN_MAXSIZE);
	real_req_len = req_len;
	/* XXX this part is missing in saux string, but its present
	 * in the header afterwards
	 */
	req_len += strlen("paste_code=");

	/* Generate options */
	snprintf(req_options, sizeof(req_options) - 1,
	    "&paste_expire_date=%s"
	    "&paste_private=%s"
	    "&paste_format=%s",
	    pb_conv_expire_date(ctx->duration),
	    pb_conv_privacy(ctx->privacy),
	    ctx->type->name);
	req_options[sizeof(req_options) - 1] = 0;
	req_opt_len = strlen(req_options);

	total_len = req_len + req_opt_len;

	/* XXX generate HTTP post string*/
	snprintf(req_header, sizeof(req_header) - 1,
	    "POST %s HTTP/1.1\r\n"
	    "Host: %s\r\n"
	    "Content-Length: %lu\r\n"
	    "Content-Type: application/x-www-form-urlencoded\r\n"
	    "\r\npaste_code=",
	    PASTEBIN_URL, PASTEBIN_HOST, total_len);
	req_header[sizeof(req_header) - 1] = 0;
	hdr_len = strlen(req_header);

	buffer = malloc(real_req_len + hdr_len);
	if (buffer == NULL) {
		close(sd);
		return (NULL);
	}

	bufaux = buffer;
	total_len = 0;

	memcpy(bufaux, req_header, hdr_len);
	bufaux += hdr_len;
	total_len += hdr_len;

	memcpy(bufaux, saux, real_req_len);
	bufaux += real_req_len;
	total_len += real_req_len;

	memcpy(bufaux, req_options, req_opt_len);
	bufaux += req_opt_len;
	total_len += req_opt_len;

	*bufaux = 0;
	fprintf(stderr, "\n[%s]\n", buffer);

	/* Send HTTP post to pastebin.com */
	ret = write(sd, buffer, total_len);
	if (ret < ((ssize_t) total_len))
		fprintf(stderr,
		    "WARNING: Paste might not be entirely sent.\n"
		    "(%lu of %ld)\n", ret, total_len);

	/* XXX use a known fixed size to get answers */
	free(buffer);
	buffer = malloc(BUFSIZ);
	if (buffer == NULL) {
		close(sd);
		return (NULL);
	}

	ret = read(sd, buffer, BUFSIZ);
	close(sd);
	if (ret == -1) {
		warn("read()");
		free(buffer);
		return (NULL);
	}
	/* TODO parse and return only the paste URL */

	/* Only use what we need */
	answer = realloc(buffer, (ret + 1));
	if (answer == NULL) {
		free(buffer);
		return (NULL);
	}

	answer[ret] = 0;
	return (answer);
}

static const char *
pb_conv_expire_date(enum pb_paste_duration dur)
{
	switch (dur) {
	case PB_FOREVER:
		return ("N");

	case PB_TEN_MINUTES:
		return ("10M");

	case PB_ONE_HOUR:
		return ("1H");

	case PB_ONE_DAY:
		return ("1D");

	case PB_ONE_MONTH:
		return ("1M");

	default:
		return("10M");
	}
}

static const char *
pb_conv_privacy(enum pb_privacy_setting set)
{
	switch (set) {
	case PB_PUBLIC:
		return ("0");

	case PB_UNLISTED:
		return ("1");

	case PB_PRIVATE:
		return ("2");

	default:
		return ("1");
	}
}

enum pb_paste_duration
pb_duration(struct paste_ctx *pctx, enum pb_paste_duration dur)
{
	struct pastebin_ctx	*ctx;

	ctx = pctx->data;

	if (dur == PB_GET_DURATION
	    || (dur < 0 || dur > 4))
		return (ctx->duration);

	ctx->duration = dur;

	return (dur);
}

enum pb_privacy_setting
pb_privacy(struct paste_ctx *pctx, enum pb_privacy_setting set)
{
	struct pastebin_ctx	*ctx;

	ctx = pctx->data;

	if (set == PB_GET_PRIVACY
	    || (set < 0 || set > 2))
		return (ctx->privacy);

	ctx->privacy = set;

	return (set);
}

static struct paste_format *
pb_paste_format_by_name(const char *str)
{
	int	 i;

	for (i = 0; paste_format_list[i].key != 0; i++)
		if (strcmp(str, paste_format_list[i].name) == 0)
			return (&paste_format_list[i]);

	return (&paste_format_list[120]);
}

static struct paste_format *
pb_paste_format_by_index(const int key)
{
	int	 i;

	for (i = 0; paste_format_list[i].key != 0; i++)
		if (paste_format_list[i].key == key)
			return (&paste_format_list[i]);

	return (&paste_format_list[120]);
}

void
pb_paste_format_name(struct paste_ctx *pctx, const char *str)
{
	struct pastebin_ctx	*ctx;
	struct paste_format	*pf;

	ctx = pctx->data;

	pf = pb_paste_format_by_name(str);

	ctx->type = pf;
}

void
pb_paste_format_id(struct paste_ctx *pctx, const int id)
{
	struct pastebin_ctx	*ctx;
	struct paste_format	*pf;

	ctx = pctx->data;

	pf = pb_paste_format_by_index(id);

	ctx->type = pf;
}
