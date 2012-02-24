#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lib/urlcode.h>

/* Converts a hex character to its integer value */
static char from_hex(char);
/* Converts an integer value to its hex character*/
static char to_hex(char);


static char from_hex(const char ch) {
  return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

static char to_hex(const char code) {
  static char hex[] = "0123456789abcdef";
  return hex[code & 15];
}

char *url_encode(const char *str) {
  char *buf, *pbuf;
  const char *pstr;

  pstr = str;
  buf = malloc(strlen(str) * 3 + 1);
  pbuf = buf;
  if (pbuf == NULL)
	  return (NULL);

  while (*pstr) {
    if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') 
      *pbuf++ = *pstr;
    else if (*pstr == ' ') 
      *pbuf++ = '+';
    else 
      *pbuf++ = '%', *pbuf++ = to_hex(*pstr >> 4), *pbuf++ = to_hex(*pstr & 15);
    pstr++;
  }
  *pbuf = '\0';
  return buf;
}

char *url_decode(const char *str) {
  char *buf, *pbuf;
  const char *pstr;

  pstr = str;
  buf = malloc(strlen(str) + 1);
  pbuf = buf;
  if (pbuf == NULL)
	  return (NULL);

  while (*pstr) {
    if (*pstr == '%') {
      if (pstr[1] && pstr[2]) {
        *pbuf++ = from_hex(pstr[1]) << 4 | from_hex(pstr[2]);
        pstr += 2;
      }
    } else if (*pstr == '+') { 
      *pbuf++ = ' ';
    } else {
      *pbuf++ = *pstr;
    }
    pstr++;
  }
  *pbuf = '\0';
  return buf;
}
