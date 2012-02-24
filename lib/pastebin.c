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
	const char	 type[64];
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

	enum paste_duration	 duration;
	struct paste_format	*type;
};

char	*send_cb_pastebin(struct paste_ctx *, const u_char *);

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
	ctx->type = &paste_format_list[27]; /* defaults to 'c' */

	return (new);
}

char *
send_cb_pastebin(struct paste_ctx *pctx, const u_char *buf)
{
	struct pastebin_ctx	*ctx;
	struct addrinfo		*res, *it;
	struct addrinfo		 hints;
	int			 sd, status;
	char			*buffer, *answer;
	char			*saux, *bufaux;
	char			 req_header[256];
	size_t			 hdr_len, req_len, total_len;
	ssize_t			 ret;

	ctx = pctx->data;
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
	}
	freeaddrinfo(res);

	if (status != 0)
		return (NULL);

	saux = url_encode(buf);
	if (saux == NULL) {
		close(sd);
		return (NULL);
	}

	req_len = strnlen(saux, PASTEBIN_MAXSIZE);
	snprintf(req_header, sizeof(req_header) - 1,
	    "POST %s HTTP/1.1\r\n"
	    "Host: %s\r\n"
	    "Content-Length: %lu\r\n"
	    "Content-Type: application/x-www-form-urlencoded\r\n"
	    "\r\npaste_code='",
	    PASTEBIN_URL, PASTEBIN_HOST, req_len);
	req_header[sizeof(req_header) - 1] = 0;
	hdr_len = strlen(req_header);

	buffer = malloc(req_len + hdr_len);
	if (buffer == NULL) {
		close(sd);
		return (NULL);
	}

	bufaux = buffer;
	total_len = 0;

	memcpy(bufaux, req_header, hdr_len);
	bufaux += hdr_len;
	total_len += hdr_len;

	memcpy(bufaux, saux, req_len);
	bufaux += req_len;
	total_len += req_len;

	*bufaux = '\'';
	bufaux++;
	*bufaux = 0;
	total_len += 2;
	fprintf(stderr, "\n[%s]\n", buffer);

	ret = write(sd, buffer, total_len);
	if (ret < ((ssize_t) total_len))
		fprintf(stderr,
		    "WARNING: Paste might not be entirely sent.\n"
		    "(%lu of %ld)\n", ret, total_len);

	ret = read(sd, buffer, total_len);
	close(sd);
	if (ret == -1) {
		warn("read()");
		free(buffer);
		return (NULL);
	}

	answer = realloc(buffer, (ret + 1));
	if (answer == NULL) {
		free(buffer);
		return (NULL);
	}

	answer[ret] = 0;
	return (answer);
}
