#ifndef __URLCODE__
#define __URLCODE__

/* rzalamena:
 * XXX this code was shamelessly took from:
 * http://www.geekhideout.com/urlcode.shtml
 */

/* Returns a url-encoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_encode(const char *);

/* Returns a url-decoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_decode(const char *);

#endif /* __URLCODE__ */
