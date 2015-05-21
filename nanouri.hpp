/*
 * The MIT License (MIT)
 * Copyright (C) 2015 Tokuhiro Matsuno, http://64p.org/ <tokuhirom@gmail.com>
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
#ifndef NANOURI_HPP_
#define NANOURI_HPP_

#include "nanouri.h"

#include <cstdlib>
#include <string>

#ifdef __GNUC__
#define NANOURI_DECLARE __attribute__((__used__))
#else
#define NANOURI_DECLARE
#endif

namespace nanouri {
static NANOURI_DECLARE std::string escape_uri(const std::string &src) {
  std::string dst;
  dst.reserve(src.size() * 3 + 1);
  for (unsigned int i = 0; i < src.size(); i++) {
    if (nanouri_isuric((unsigned char) src[i])) {
      dst += '%';
      dst += nanouri_hex_char((src[i] >> 4) & 0x0f);
      dst += nanouri_hex_char(src[i] & 0x0f);
    } else {
      dst += src[i];
    }
  }
  return dst;
}

static int unhex(unsigned char c) {
  return (c >= '0' && c <= '9') ? c - '0' : (c >= 'A' && c <= 'F')
             ? c - 'A' + 10
             : c - 'a' + 10;
}

static NANOURI_DECLARE std::string unescape_uri(const std::string &src) {
  std::string dst;
  dst.reserve(src.size() * 3 + 1);
  for (unsigned int i = 0; i < src.size(); i++) {
    if (src[i] == '%') {
      unsigned char c;
      if (src[++i] != '\0') {
        c = unhex(src[i]) << 4;
      }
      if (src[++i] != '\0') {
        c += unhex(src[i]);
      }
      dst += c;
    } else {
      dst += src[i];
    }
  }
  return dst;
}

class Uri {
 private:
  std::string uri_;
  std::string host_;
  std::string scheme_;
  int port_;
  std::string path_query_;

 public:
  Uri() {}
  ~Uri() {}

  /**
    * @return true if valid url
    */
  inline bool parse(const std::string &src) {
    return this->parse(src.c_str(), src.size());
  }
  bool parse(const char *src, size_t src_len) {
    const char *scheme;
    size_t scheme_len;
    const char *host;
    size_t host_len;
    const char *path_query;
    int path_query_len;
    int ret =
        nanouri_parse_uri(src, src_len, &scheme, &scheme_len, &host, &host_len,
                          &port_, &path_query, &path_query_len);
    if (ret != 0) {
      return false;  // parse error
    }
    uri_.assign(src, src_len);
    host_.assign(host, host_len);
    path_query_.assign(path_query, path_query_len);
    scheme_.assign(scheme, scheme_len);
    return true;
  }
  inline std::string host() { return host_; }
  inline std::string scheme() { return scheme_; }
  inline int port() { return port_; }
  inline std::string path_query() { return path_query_; }
  inline std::string as_string() { return uri_; }
  operator bool() const { return !this->uri_.empty(); }
};
};

#undef NANOURI_DECLARE

#endif  // NANOURI_HPP_
