// Copyright (C) 2017 Tomoyuki Fujimori <moyu@dromozoa.com>
//
// This file is part of dromozoa-curl.
//
// dromozoa-curl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// dromozoa-curl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with dromozoa-curl.  If not, see <http://www.gnu.org/licenses/>.

#include "common.hpp"

namespace dromozoa {
  void initialize_main(lua_State* L) {
    // BEHAVIOR OPTIONS
    luaX_set_field<int>(L, -1, "CURLOPT_VERBOSE", CURLOPT_VERBOSE);
    luaX_set_field<int>(L, -1, "CURLOPT_HEADER", CURLOPT_HEADER);
    luaX_set_field<int>(L, -1, "CURLOPT_NOPROGRESS", CURLOPT_NOPROGRESS);
    luaX_set_field<int>(L, -1, "CURLOPT_NOSIGNAL", CURLOPT_NOSIGNAL);
    luaX_set_field<int>(L, -1, "CURLOPT_WILDCARDMATCH", CURLOPT_WILDCARDMATCH);

    // ERROR OPTIONS
    luaX_set_field<int>(L, -1, "CURLOPT_ERRORBUFFER", CURLOPT_ERRORBUFFER);
    luaX_set_field<int>(L, -1, "CURLOPT_STDERR", CURLOPT_STDERR);
    luaX_set_field<int>(L, -1, "CURLOPT_FAILONERROR", CURLOPT_FAILONERROR);
#if CURL_AT_LEAST_VERSION(7,51,0)
    luaX_set_field<int>(L, -1, "CURLOPT_KEEP_SENDING_ON_ERROR", CURLOPT_KEEP_SENDING_ON_ERROR);
#endif

    // NETWORK OPTIONS
    luaX_set_field<int>(L, -1, "CURLOPT_URL", CURLOPT_URL);
    luaX_set_field<int>(L, -1, "CURLOPT_PATH_AS_IS", CURLOPT_PATH_AS_IS);
    luaX_set_field<int>(L, -1, "CURLOPT_REDIR_PROTOCOLS", CURLOPT_REDIR_PROTOCOLS);
#if CURL_AT_LEAST_VERSION(7,45,0)
    luaX_set_field<int>(L, -1, "CURLOPT_DEFAULT_PROTOCOL", CURLOPT_DEFAULT_PROTOCOL);
#endif
    luaX_set_field<int>(L, -1, "CURLOPT_PROXY", CURLOPT_PROXY);
#if CURL_AT_LEAST_VERSION(7,52,0)
    luaX_set_field<int>(L, -1, "CURLOPT_PRE_PROXY", CURLOPT_PRE_PROXY);
#endif
    luaX_set_field<int>(L, -1, "CURLOPT_PROXYPORT", CURLOPT_PROXYPORT);
    luaX_set_field<int>(L, -1, "CURLOPT_PROXYTYPE", CURLOPT_PROXYTYPE);
    luaX_set_field<int>(L, -1, "CURLOPT_NOPROXY", CURLOPT_NOPROXY);
    luaX_set_field<int>(L, -1, "CURLOPT_HTTPPROXYTUNNEL", CURLOPT_HTTPPROXYTUNNEL);
#if CURL_AT_LEAST_VERSION(7,49,0)
    luaX_set_field<int>(L, -1, "CURLOPT_CONNECT_TO", CURLOPT_CONNECT_TO);
#endif
    luaX_set_field<int>(L, -1, "CURLOPT_SOCKS5_GSSAPI_SERVICE", CURLOPT_SOCKS5_GSSAPI_SERVICE);
    luaX_set_field<int>(L, -1, "CURLOPT_SOCKS5_GSSAPI_NEC", CURLOPT_SOCKS5_GSSAPI_NEC);
    luaX_set_field<int>(L, -1, "CURLOPT_PROXY_SERVICE_NAME", CURLOPT_PROXY_SERVICE_NAME);
    luaX_set_field<int>(L, -1, "CURLOPT_SERVICE_NAME", CURLOPT_SERVICE_NAME);
    luaX_set_field<int>(L, -1, "CURLOPT_INTERFACE", CURLOPT_INTERFACE);
    luaX_set_field<int>(L, -1, "CURLOPT_LOCALPORT", CURLOPT_LOCALPORT);
    luaX_set_field<int>(L, -1, "CURLOPT_LOCALPORTRANGE", CURLOPT_LOCALPORTRANGE);
    luaX_set_field<int>(L, -1, "CURLOPT_DNS_CACHE_TIMEOUT", CURLOPT_DNS_CACHE_TIMEOUT);
    luaX_set_field<int>(L, -1, "CURLOPT_DNS_USE_GLOBAL_CACHE", CURLOPT_DNS_USE_GLOBAL_CACHE);
    luaX_set_field<int>(L, -1, "CURLOPT_BUFFERSIZE", CURLOPT_BUFFERSIZE);
    luaX_set_field<int>(L, -1, "CURLOPT_PORT", CURLOPT_PORT);
#if CURL_AT_LEAST_VERSION(7,49,0)
    luaX_set_field<int>(L, -1, "CURLOPT_TCP_FASTOPEN", CURLOPT_TCP_FASTOPEN);
#endif
    luaX_set_field<int>(L, -1, "CURLOPT_TCP_NODELAY", CURLOPT_TCP_NODELAY);
    luaX_set_field<int>(L, -1, "CURLOPT_ADDRESS_SCOPE", CURLOPT_ADDRESS_SCOPE);
    luaX_set_field<int>(L, -1, "CURLOPT_TCP_KEEPALIVE", CURLOPT_TCP_KEEPALIVE);
    luaX_set_field<int>(L, -1, "CURLOPT_TCP_KEEPIDLE", CURLOPT_TCP_KEEPIDLE);
    luaX_set_field<int>(L, -1, "CURLOPT_TCP_KEEPINTVL", CURLOPT_TCP_KEEPINTVL);
    luaX_set_field<int>(L, -1, "CURLOPT_UNIX_SOCKET_PATH", CURLOPT_UNIX_SOCKET_PATH);
#if CURL_AT_LEAST_VERSION(7,53,0)
    luaX_set_field<int>(L, -1, "CURLOPT_ABSTRACT_UNIX_SOCKET", CURLOPT_ABSTRACT_UNIX_SOCKET);
#endif

    // NAMES and PASSWORDS OPTIONS (Authentication)
    luaX_set_field<int>(L, -1, "CURLOPT_NETRC", CURLOPT_NETRC);
    luaX_set_field<int>(L, -1, "CURLOPT_NETRC_FILE", CURLOPT_NETRC_FILE);
    luaX_set_field<int>(L, -1, "CURLOPT_USERPWD", CURLOPT_USERPWD);
    luaX_set_field<int>(L, -1, "CURLOPT_PROXYUSERPWD", CURLOPT_PROXYUSERPWD);
    luaX_set_field<int>(L, -1, "CURLOPT_USERNAME", CURLOPT_USERNAME);
    luaX_set_field<int>(L, -1, "CURLOPT_PASSWORD", CURLOPT_PASSWORD);
    luaX_set_field<int>(L, -1, "CURLOPT_LOGIN_OPTIONS", CURLOPT_LOGIN_OPTIONS);
    luaX_set_field<int>(L, -1, "CURLOPT_PROXYUSERNAME", CURLOPT_PROXYUSERNAME);
    luaX_set_field<int>(L, -1, "CURLOPT_PROXYPASSWORD", CURLOPT_PROXYPASSWORD);
    luaX_set_field<int>(L, -1, "CURLOPT_HTTPAUTH", CURLOPT_HTTPAUTH);
    luaX_set_field<int>(L, -1, "CURLOPT_TLSAUTH_USERNAME", CURLOPT_TLSAUTH_USERNAME);
#if CURL_AT_LEAST_VERSION(7,52,0)
    luaX_set_field<int>(L, -1, "CURLOPT_PROXY_TLSAUTH_USERNAME", CURLOPT_PROXY_TLSAUTH_USERNAME);
#endif
    luaX_set_field<int>(L, -1, "CURLOPT_TLSAUTH_PASSWORD", CURLOPT_TLSAUTH_PASSWORD);
#if CURL_AT_LEAST_VERSION(7,52,0)
    luaX_set_field<int>(L, -1, "CURLOPT_PROXY_TLSAUTH_PASSWORD", CURLOPT_PROXY_TLSAUTH_PASSWORD);
#endif
    luaX_set_field<int>(L, -1, "CURLOPT_TLSAUTH_TYPE", CURLOPT_TLSAUTH_TYPE);
#if CURL_AT_LEAST_VERSION(7,52,0)
    luaX_set_field<int>(L, -1, "CURLOPT_PROXY_TLSAUTH_TYPE", CURLOPT_PROXY_TLSAUTH_TYPE);
#endif
    luaX_set_field<int>(L, -1, "CURLOPT_PROXYAUTH", CURLOPT_PROXYAUTH);
    luaX_set_field<int>(L, -1, "CURLOPT_SASL_IR", CURLOPT_SASL_IR);
    luaX_set_field<int>(L, -1, "CURLOPT_XOAUTH2_BEARER", CURLOPT_XOAUTH2_BEARER);

    // HTTP OPTIONS
    luaX_set_field<int>(L, -1, "CURLOPT_AUTOREFERER", CURLOPT_AUTOREFERER);
    luaX_set_field<int>(L, -1, "CURLOPT_ACCEPT_ENCODING", CURLOPT_ACCEPT_ENCODING);
    luaX_set_field<int>(L, -1, "CURLOPT_TRANSFER_ENCODING", CURLOPT_TRANSFER_ENCODING);
    luaX_set_field<int>(L, -1, "CURLOPT_FOLLOWLOCATION", CURLOPT_FOLLOWLOCATION);
    luaX_set_field<int>(L, -1, "CURLOPT_UNRESTRICTED_AUTH", CURLOPT_UNRESTRICTED_AUTH);
    luaX_set_field<int>(L, -1, "CURLOPT_MAXREDIRS", CURLOPT_MAXREDIRS);
    luaX_set_field<int>(L, -1, "CURLOPT_POSTREDIR", CURLOPT_POSTREDIR);
    luaX_set_field<int>(L, -1, "CURLOPT_PUT", CURLOPT_PUT);
    luaX_set_field<int>(L, -1, "CURLOPT_POST", CURLOPT_POST);
    luaX_set_field<int>(L, -1, "CURLOPT_POSTFIELDS", CURLOPT_POSTFIELDS);
    luaX_set_field<int>(L, -1, "CURLOPT_POSTFIELDSIZE", CURLOPT_POSTFIELDSIZE);
    luaX_set_field<int>(L, -1, "CURLOPT_POSTFIELDSIZE_LARGE", CURLOPT_POSTFIELDSIZE_LARGE);
    luaX_set_field<int>(L, -1, "CURLOPT_COPYPOSTFIELDS", CURLOPT_COPYPOSTFIELDS);
    luaX_set_field<int>(L, -1, "CURLOPT_HTTPPOST", CURLOPT_HTTPPOST);
    luaX_set_field<int>(L, -1, "CURLOPT_REFERER", CURLOPT_REFERER);
    luaX_set_field<int>(L, -1, "CURLOPT_USERAGENT", CURLOPT_USERAGENT);
    luaX_set_field<int>(L, -1, "CURLOPT_HTTPHEADER", CURLOPT_HTTPHEADER);
    luaX_set_field<int>(L, -1, "CURLOPT_HEADEROPT", CURLOPT_HEADEROPT);
    luaX_set_field<int>(L, -1, "CURLOPT_PROXYHEADER", CURLOPT_PROXYHEADER);
    luaX_set_field<int>(L, -1, "CURLOPT_HTTP200ALIASES", CURLOPT_HTTP200ALIASES);
    luaX_set_field<int>(L, -1, "CURLOPT_COOKIE", CURLOPT_COOKIE);
    luaX_set_field<int>(L, -1, "CURLOPT_COOKIEFILE", CURLOPT_COOKIEFILE);
    luaX_set_field<int>(L, -1, "CURLOPT_COOKIEJAR", CURLOPT_COOKIEJAR);
    luaX_set_field<int>(L, -1, "CURLOPT_COOKIESESSION", CURLOPT_COOKIESESSION);
    luaX_set_field<int>(L, -1, "CURLOPT_COOKIELIST", CURLOPT_COOKIELIST);
    luaX_set_field<int>(L, -1, "CURLOPT_HTTPGET", CURLOPT_HTTPGET);
    luaX_set_field<int>(L, -1, "CURLOPT_HTTP_VERSION", CURLOPT_HTTP_VERSION);
    luaX_set_field<int>(L, -1, "CURLOPT_IGNORE_CONTENT_LENGTH", CURLOPT_IGNORE_CONTENT_LENGTH);
    luaX_set_field<int>(L, -1, "CURLOPT_HTTP_CONTENT_DECODING", CURLOPT_HTTP_CONTENT_DECODING);
    luaX_set_field<int>(L, -1, "CURLOPT_HTTP_TRANSFER_DECODING", CURLOPT_HTTP_TRANSFER_DECODING);
    luaX_set_field<int>(L, -1, "CURLOPT_EXPECT_100_TIMEOUT_MS", CURLOPT_EXPECT_100_TIMEOUT_MS);
    luaX_set_field<int>(L, -1, "CURLOPT_PIPEWAIT", CURLOPT_PIPEWAIT);
#if CURL_AT_LEAST_VERSION(7,46,0)
    luaX_set_field<int>(L, -1, "CURLOPT_STREAM_DEPENDS", CURLOPT_STREAM_DEPENDS);
    luaX_set_field<int>(L, -1, "CURLOPT_STREAM_DEPENDS_E", CURLOPT_STREAM_DEPENDS_E);
    luaX_set_field<int>(L, -1, "CURLOPT_STREAM_WEIGHT", CURLOPT_STREAM_WEIGHT);
#endif

    // SMTP OPTIONS
    luaX_set_field<int>(L, -1, "CURLOPT_MAIL_FROM", CURLOPT_MAIL_FROM);
    luaX_set_field<int>(L, -1, "CURLOPT_MAIL_RCPT", CURLOPT_MAIL_RCPT);
    luaX_set_field<int>(L, -1, "CURLOPT_MAIL_AUTH", CURLOPT_MAIL_AUTH);

    // TFTP OPTIONS
    luaX_set_field<int>(L, -1, "CURLOPT_TFTP_BLKSIZE", CURLOPT_TFTP_BLKSIZE);
#if CURL_AT_LEAST_VERSION(7,48,0)
    luaX_set_field<int>(L, -1, "CURLOPT_TFTP_NO_OPTIONS", CURLOPT_TFTP_NO_OPTIONS);
#endif

    // FTP OPTIONS
    luaX_set_field<int>(L, -1, "CURLOPT_FTPPORT", CURLOPT_FTPPORT);
    luaX_set_field<int>(L, -1, "CURLOPT_QUOTE", CURLOPT_QUOTE);
    luaX_set_field<int>(L, -1, "CURLOPT_POSTQUOTE", CURLOPT_POSTQUOTE);
    luaX_set_field<int>(L, -1, "CURLOPT_PREQUOTE", CURLOPT_PREQUOTE);
    luaX_set_field<int>(L, -1, "CURLOPT_APPEND", CURLOPT_APPEND);
    luaX_set_field<int>(L, -1, "CURLOPT_FTP_USE_EPRT", CURLOPT_FTP_USE_EPRT);
    luaX_set_field<int>(L, -1, "CURLOPT_FTP_USE_EPSV", CURLOPT_FTP_USE_EPSV);
    luaX_set_field<int>(L, -1, "CURLOPT_FTP_USE_PRET", CURLOPT_FTP_USE_PRET);
    luaX_set_field<int>(L, -1, "CURLOPT_FTP_CREATE_MISSING_DIRS", CURLOPT_FTP_CREATE_MISSING_DIRS);
    luaX_set_field<int>(L, -1, "CURLOPT_FTP_RESPONSE_TIMEOUT", CURLOPT_FTP_RESPONSE_TIMEOUT);
    luaX_set_field<int>(L, -1, "CURLOPT_FTP_ALTERNATIVE_TO_USER", CURLOPT_FTP_ALTERNATIVE_TO_USER);
    luaX_set_field<int>(L, -1, "CURLOPT_FTP_SKIP_PASV_IP", CURLOPT_FTP_SKIP_PASV_IP);
    luaX_set_field<int>(L, -1, "CURLOPT_FTPSSLAUTH", CURLOPT_FTPSSLAUTH);
    luaX_set_field<int>(L, -1, "CURLOPT_FTP_SSL_CCC", CURLOPT_FTP_SSL_CCC);
    luaX_set_field<int>(L, -1, "CURLOPT_FTP_ACCOUNT", CURLOPT_FTP_ACCOUNT);
    luaX_set_field<int>(L, -1, "CURLOPT_FTP_FILEMETHOD", CURLOPT_FTP_FILEMETHOD);

    // luaX_set_field<int>(L, -1, "", );
  }
}
