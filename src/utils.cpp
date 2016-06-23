/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "utils.h"

#include <boost/locale.hpp>

v8::Local<v8::String> NodeLocalString(const std::wstring& str) {
  return Nan::New(WstrToStr(str)).ToLocalChecked();
}

bool NodeStringParam(Nan::NAN_METHOD_ARGS_TYPE info, int index, const std::wstring& name, std::wstring& out) {
  if (info.Length() < index + 1 || info[index]->IsUndefined()) {
    Nan::ThrowError(NodeLocalString(name + L" must be provided"));
    return false;
  }
  if (!info[index]->IsString()) {
    Nan::ThrowTypeError(NodeLocalString(name + L" must be a string"));
    return false;
  }
  out = StrToWstr(*Nan::Utf8String(info[index]));
  return true;
}

bool NodeCallbackParam(Nan::NAN_METHOD_ARGS_TYPE info, int index, const std::wstring& name, Nan::Callback*& out) {
  if (info.Length() < index + 1 || info[index]->IsUndefined()) {
    Nan::ThrowError(NodeLocalString(name + L" must be provided"));
    return false;
  }
  if (!info[index]->IsFunction()) {
    Nan::ThrowTypeError(NodeLocalString(name + L" must be a function"));
    return false;
  }
  out = new Nan::Callback(info[index].As<v8::Function>());
  return true;
}

std::string WstrToStr(const std::wstring& input) {
  return boost::locale::conv::utf_to_utf<char>(input);
}

std::wstring StrToWstr(const std::string& input) {
  return boost::locale::conv::utf_to_utf<wchar_t>(input);
}
