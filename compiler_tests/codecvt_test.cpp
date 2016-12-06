/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <cassert>
#include <codecvt>
#include <iostream>
#include <locale>

typedef std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wstr_conv;


std::string WstrToStr(const std::wstring &input) {
  static wstr_conv conv;
  return conv.to_bytes(input);
}

std::wstring StrToWstr(const std::string &input) {
  static wstr_conv conv;
  return conv.from_bytes(input);
}

int main(void) {
  std::locale::global(std::locale(""));
  std::wcout.imbue(std::locale());
  std::wstring wstr = L"こんいちは、i 8 Σ π";
  std::string str = WstrToStr(wstr);
  std::wstring wstr2 = StrToWstr(str);
  assert(wstr == wstr2);
  std::wcout << wstr << std::endl << wstr2 << std::endl;
}
