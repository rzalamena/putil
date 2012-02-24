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

#ifndef __PASTEBIN__
#define __PASTEBIN__

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN	256
#endif

#define PASTEBIN_MAXSIZE	524288	/* 0.5MB (http://pastebin.com/faq) */
#define PASTEBIN_HOST		"pastebin.com"
#define PASTEBIN_URL		"/api_public.php"
#define PASTEBIN_KEY_URL	"/api/api_post.php"

enum paste_duration {
	PB_FOREVER,	/* 'N'   */
	PB_TEN_MINUTES,	/* '10M' */
	PB_ONE_HOUR,	/* '1H'  */
	PB_ONE_DAY,	/* '1D'  */
	PB_ONE_MONTH,	/* '1M'  */
};

enum privacy_setting {
	PB_PUBLIC,
	PB_UNLISTED,
	PB_PRIVATE,
};

struct paste_ctx	*new_pb_paste(void);

#endif /* __PASTEBIN__ */
