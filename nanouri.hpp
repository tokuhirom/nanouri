/*
 * Copyright (c) 2009, 2011, tokuhiro matsuno
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
