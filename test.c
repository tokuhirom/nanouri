#include <stdio.h>
#include <string.h>
#include "picouri.h"
#include "tap.h"

int main () {
    const char * scheme;
    size_t scheme_len;
    const char * host;
    size_t host_len;
    int port;
    const char *path_query;
    int path_query_len;

#define TEST(uri, expect_scheme, expect_host, expect_port, expect_path_query) \
    do { \
        diag(uri); \
        int ret = parse_uri(uri, strlen(uri), &scheme, &scheme_len, &host, &host_len, &port, &path_query, &path_query_len); \
        ok(ret == 0, "retval"); \
        ok(strncmp(expect_scheme, scheme, scheme_len) == 0, "scheme"); \
        ok(strncmp(expect_host, host, host_len) == 0, "host"); \
        ok(port == expect_port, "port"); \
        ok(strncmp(expect_path_query, path_query, path_query_len) == 0, "path_query"); \
    } while (0)

#define TEST_ERROR(uri, expect_ret) \
    do { \
        diag(uri); \
        int ret = parse_uri(uri, strlen(uri), &scheme, &scheme_len, &host, &host_len, &port, &path_query, &path_query_len); \
        ok(ret == expect_ret, "retval"); \
    } while (0)

    TEST("http://mixi.jp:9090/foo=?bar=baz", "http", "mixi.jp", 9090, "/foo=?bar=baz");
    TEST("http://d.hatena.ne.jp", "http", "d.hatena.ne.jp", 0, "");
    TEST("http://d.hatena.ne.jp:80", "http", "d.hatena.ne.jp", 80, "");
    TEST("http://d.hatena.ne.jp/", "http", "d.hatena.ne.jp", 0, "/");
    TEST_ERROR("http", -2);
    TEST_ERROR("http:-", -1);
    TEST_ERROR("http://mixi.jp:<", -1);

    done_testing();

    return 0;
}

