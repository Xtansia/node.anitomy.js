/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <locale>
#include <codecvt>
#include "util.h"

static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utf8To16Conv;

std::wstring StrToWstr(const std::string& input) {
  return utf8To16Conv.from_bytes(input);
}

std::string WstrToStr(const std::wstring& input) {
  return utf8To16Conv.to_bytes(input);
}

NAN_SETTER(READ_ONLY_SETTER) {
  std::string name = *Nan::Utf8String(property);
  std::string err = name + " is a read-only property";
  Nan::ThrowError(err.c_str());
}