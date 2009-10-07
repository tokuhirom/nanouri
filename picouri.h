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

/* scheme part and host part were already to get */
#define CHECK_EOF_NO_ERROR() \
  if (buf == buf_end) { \
    *path_query_len = 0; \
    return 0;      \
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
    *scheme_len = buf - *scheme - 1;

    EXPECT(':'); EXPECT('/'); EXPECT('/');

    *host = buf;
    *port = 0;
    for (;;++buf) {
        CHECK_EOF_NO_ERROR();
        if (':' == *buf) { /* with port */
            *host_len = buf - *host - 1;
            buf++;

            *port = 0;
            for (;'0' <= *buf && *buf <= '9';buf++) {
                CHECK_EOF_NO_ERROR();
                *port = *port * 10 + (*buf - '0');
            }
            CHECK_EOF_NO_ERROR();
            EXPECT('/');
            break;
        }
        if ('/' == *buf) { /* no port */
            *host_len = buf - *host - 1;
            break;
        }
    }

    *path_query = buf-1;
    *path_query_len = buf_end - buf;
    return 0;
}

#undef EXPECT
#undef CHECK_EOF
#endif /* PICOURI_H */
