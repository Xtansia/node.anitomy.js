/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once
#ifndef ANITOMYJS_UTIL_H
#define ANITOMYJS_UTIL_H

#include <string>
#include <nan.h>

std::wstring StrToWstr(const std::string& input);
std::string WstrToStr(const std::wstring& input);

NAN_SETTER(READ_ONLY_SETTER);

#define NODE_UNWRAP_OBJ(Type) \
  Type* obj = ObjectWrap::Unwrap<Type>(info.Holder());

#define NODE_LOCAL_STRING(str) \
  Nan::New(str).ToLocalChecked()

#define NODE_LOCAL_WSTRING(str) \
  Nan::New(WstrToStr(str)).ToLocalChecked()

#define NODE_ENSURE_VALUE_BOOLEAN(name, value) \
  if (!value->IsBoolean()) { \
    Nan::ThrowTypeError(#name " must be a boolean"); \
    return; \
  }

#define NODE_ENSURE_VALUE_STRING(name, value) \
  if (!value->IsString()) { \
    Nan::ThrowTypeError(#name " must be a string"); \
    return; \
  }

#define NODE_STRING_ARG(i, name) \
  if (info.Length() < i + 1) { \
    Nan::ThrowError(#name " must be given"); \
    return; \
  } \
  NODE_ENSURE_VALUE_STRING(name, info[i]); \
  std::wstring name = StrToWstr(*Nan::Utf8String(info[i]));

#define NODE_GETTER_AND_SETTER(name) \
  static NAN_GETTER(Get##name); \
  static NAN_SETTER(Set##name);

#define NODE_SET_ACCESSOR(jsName, cppName) \
  Nan::SetAccessor(tpl->InstanceTemplate(), NODE_LOCAL_STRING(jsName), Get##cppName, Set##cppName);

#endif // !ANITOMYJS_UTIL_H