/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <boost/locale/encoding.hpp>
#include "util.h"

std::wstring StrToWstr(const std::string& input) {
  return boost::locale::conv::utf_to_utf<wchar_t>(input);
}

std::string WstrToStr(const std::wstring& input) {
  return boost::locale::conv::utf_to_utf<char>(input);
}

NAN_SETTER(READ_ONLY_SETTER) {
  std::string name = *Nan::Utf8String(property);
  std::string err = name + " is a read-only property";
  Nan::ThrowError(err.c_str());
}