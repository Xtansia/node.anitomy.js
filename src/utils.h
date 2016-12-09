/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once
#ifndef ANITOMY_LIB_UTILS_H
#define ANITOMY_LIB_UTILS_H

#include "nan_nowarn.h"
#include <anitomy/options.h>
#include <map>
#include <iterator>
#include <string>

#ifdef NODE_ANITOMY_USE_BOOST
  #include <boost/locale/encoding_utf.hpp>
#else
  #include <codecvt>
  #include <locale>
#endif

#define CONCAT_INTERNAL(X, Y) X##Y
#define CONCAT(X, Y) CONCAT_INTERNAL(X, Y)
#define STRINGIFY_INTERNAL(X) #X
#define STRINGIFY(X) STRINGIFY_INTERNAL(X)
#define WIDE_STRINGIFY(X) CONCAT(L, STRINGIFY(X))

#ifndef NODE_ANITOMY_USE_BOOST
  typedef std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wstr_conv;
#endif

inline std::string WstrToStr(const std::wstring &input) {
#ifdef NODE_ANITOMY_USE_BOOST
  return boost::locale::conv::utf_to_utf<char>(input);
#else
  static wstr_conv conv;
  return conv.to_bytes(input);
#endif
}

inline std::wstring StrToWstr(const std::string &input) {
#ifdef NODE_ANITOMY_USE_BOOST
  return boost::locale::conv::utf_to_utf<wchar_t>(input);
#else
  static wstr_conv conv;
  return conv.from_bytes(input);
#endif
}

inline v8::Local<v8::String> WstrToNode(const std::wstring &str) {
  return Nan::New(WstrToStr(str)).ToLocalChecked();
}

inline std::wstring NodeToWstr(v8::Local<v8::Value> value) {
  Nan::Utf8String str(value);
  return str.length() > 0 ? StrToWstr(*str) : L"";
}

inline bool NodeEnsureParamProvided(Nan::NAN_METHOD_ARGS_TYPE info, int index,
                                    const std::wstring &name) {
  if (info.Length() <= index || info[index]->IsUndefined()) {
    Nan::ThrowError(WstrToNode(name + L" must be provided"));
    return false;
  }

  return true;
}

bool NodeParam(Nan::NAN_METHOD_ARGS_TYPE info, int index,
               const std::wstring &name, std::wstring &out);
bool NodeParam(Nan::NAN_METHOD_ARGS_TYPE info, int index,
               const std::wstring &name, std::vector<std::wstring> &out);
bool NodeParam(Nan::NAN_METHOD_ARGS_TYPE info, int index,
               const std::wstring &name, v8::Local<v8::Function> &out);
bool NodeParam(Nan::NAN_METHOD_ARGS_TYPE info, int index,
               const std::wstring &name, v8::Local<v8::Object> &out);

inline bool NodeObjectHas(v8::Local<v8::Object> obj, const std::wstring &key) {
  return Nan::Has(obj, WstrToNode(key)).FromMaybe(false);
}

bool NodeObjectGet(v8::Local<v8::Object> obj, const std::wstring &objName,
                   const std::wstring &key, std::wstring &out);
bool NodeObjectGet(v8::Local<v8::Object> obj,
                   const std::wstring &objName, const std::wstring &key,
                   std::vector<std::wstring> &out);
bool NodeObjectGet(v8::Local<v8::Object> obj,
                   const std::wstring &objName, const std::wstring &key,
                   bool &out);

template <typename OutType>
inline bool NodeObjectGetIfHas(v8::Local<v8::Object> obj,
                               const std::wstring &objName, const std::wstring &key, OutType &out) {
  return !NodeObjectHas(obj, key) || NodeObjectGet(obj, objName, key, out);
}

template <typename KeyType, typename ValueType>
inline std::vector<ValueType> MultiMapGetAll(const
    std::multimap<KeyType, ValueType>
    &mmap, const KeyType &key) {
  auto range = mmap.equal_range(key);
  std::vector<ValueType> values;
  std::transform(range.first, range.second, std::back_inserter(values),
  [](std::pair<const KeyType, ValueType> pair) {
    return pair.second;
  });
  return values;
}

#endif // !ANITOMY_LIB_UTILS_H
