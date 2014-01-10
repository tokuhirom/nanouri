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
