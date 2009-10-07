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

