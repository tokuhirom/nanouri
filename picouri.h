/*
 * Copyright (c) 2009, tokuhiro matsuno
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * * Neither the name of the <ORGANIZATION> nor the names of its contributors
 *   may be used to endorse or promote products derived from this software
 *   without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef PICOURI_H
#define PICOURI_H

#include <stddef.h>

#define CHECK_EOF() \
  if (buf == buf_end) { \
    return -2;      \
  }

#define EXPECT(ch)    \
  CHECK_EOF();        \
  if (*buf++ != ch) { \
    return -1;        \
  }


static int parse_uri(const char* _buf, size_t len, const char** scheme, size_t *scheme_len, const char **host, size_t *host_len, int *port, const char **path_query, int*path_query_len) {
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

#undef EXPECT
#undef CHECK_EOF
#endif /* PICOURI_H */
