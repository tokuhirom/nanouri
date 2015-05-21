/*
 * The MIT License (MIT)
 * Copyright (C) 2009-2015 Tokuhiro Matsuno, http://64p.org/ <tokuhirom@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <assert.h>
#include "nanouri.h"

static char nu_uric_map[256] = 
/*  0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f */
{
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
  1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

int nanouri_isuric(unsigned char c) {
  return nu_uric_map[c];
}

char nanouri_hex_char(unsigned int n) {
  assert(n < 16);

  if (n < 10) {
    return '0'+n;
  } else {
    return 'a'+n-10;
  }
}

#define CHECK_EOF() \
  if (buf == buf_end) { \
    return -2;      \
  }

#define EXPECT(ch)    \
  CHECK_EOF();        \
  if (*buf++ != ch) { \
    return -1;        \
  }

int nanouri_parse_uri(const char* _buf, size_t len, const char** scheme, size_t *scheme_len, const char **host, size_t *host_len, int *port, const char **path_query, int*path_query_len) {
  const char * buf = _buf, * buf_end = buf + len;

  *scheme = buf;
  for (;;++buf) {
    CHECK_EOF();
    if (':' == *buf) {
      break;
    }
  }
  *scheme_len = buf - *scheme;

  EXPECT(':'); EXPECT('/'); EXPECT('/');

  *host = buf;
  *port = 0;
  *host_len = 0;
  *path_query_len = 0;
  for (;;++buf) {
    if (buf == buf_end) {
      *host_len = buf - *host;
      return 0;
    }
    if (':' == *buf) { /* with port */
      *host_len = buf - *host;
      buf++;

      *port = 0;
      for (;'0' <= *buf && *buf <= '9';buf++) {
        if (buf == buf_end) {
          return 0;
        }
        *port = *port * 10 + (*buf - '0');
      }
      if (buf == buf_end) {
        return 0;
      }
      break;
    }
    if ('/' == *buf) { /* no port */
      *host_len = buf - *host;
      break;
    }
  }

  *path_query = buf;
  *path_query_len = buf_end - buf;
  return 0;
}
