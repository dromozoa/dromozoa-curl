#ifndef DROMOZOA_SYMBOLS_HPP
#define DROMOZOA_SYMBOLS_HPP

#include "common.hpp"

namespace dromozoa {
  enum easy_setopt_param_enum {
    easy_setopt_param_unknown,
    easy_setopt_param_CURLSH_p,
    easy_setopt_param_CURL_p,
    easy_setopt_param_FILE_p,
    easy_setopt_param_callback,
    easy_setopt_param_char_p,
    easy_setopt_param_curl_mime_p,
    easy_setopt_param_curl_off_t,
    easy_setopt_param_curl_trailer_callback_p,
    easy_setopt_param_long,
    easy_setopt_param_struct_curl_httppost_p,
    easy_setopt_param_struct_curl_slist_p,
    easy_setopt_param_void_p,
  };

  enum multi_setopt_param_enum {
    multi_setopt_param_unknown,
    multi_setopt_param_callback,
    multi_setopt_param_char_pp,
    multi_setopt_param_long,
    multi_setopt_param_void_p,
  };

  void initialize_symbols(lua_State*);
  easy_setopt_param_enum easy_setopt_param(CURLoption);
  multi_setopt_param_enum multi_setopt_param(CURLMoption);
  const char* error_to_string(CURLFORMcode);
}

#endif
