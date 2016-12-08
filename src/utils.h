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
#include <string>

#define CONCAT_INTERNAL(X, Y) X##Y
#define CONCAT(X, Y) CONCAT_INTERNAL(X, Y)
#define STRINGIFY_INTERNAL(X) #X
#define STRINGIFY(X) STRINGIFY_INTERNAL(X)
#define WIDE_STRINGIFY(X) CONCAT(L, STRINGIFY(X))

std::string WstrToStr(const std::wstring &input);
std::wstring StrToWstr(const std::string &input);

inline v8::Local<v8::String> NodeLocalString(const std::wstring &str) {
  return Nan::New(WstrToStr(str)).ToLocalChecked();
}

std::wstring NodeToWstr(v8::Local<v8::Value> value);

bool NodeEnsureParamProvided(Nan::NAN_METHOD_ARGS_TYPE info, int index,
                             const std::wstring &name);
bool NodeStringOrArrayParam(Nan::NAN_METHOD_ARGS_TYPE info, int index,
                            const std::wstring &name, std::vector<std::wstring> &out);
bool NodeFunctionParam(Nan::NAN_METHOD_ARGS_TYPE info, int index,
                       const std::wstring &name, v8::Local<v8::Function> &out);
bool NodeObjectParam(Nan::NAN_METHOD_ARGS_TYPE info, int index,
                     const std::wstring &name, v8::Local<v8::Object> &out);

inline bool NodeObjectHas(v8::Local<v8::Object> obj, const std::wstring &key) {
  return Nan::Has(obj, NodeLocalString(key)).FromMaybe(false);
}

bool NodeObjectGet(v8::Local<v8::Object> obj, const std::wstring &objName,
                   const std::wstring &key, std::wstring &out);
bool NodeObjectGet(v8::Local<v8::Object> obj,
                   const std::wstring &objName, const std::wstring &key,
                   std::vector<std::wstring> &out);
bool NodeObjectGet(v8::Local<v8::Object> obj,
                   const std::wstring &objName, const std::wstring &key,
                   bool &out);

#endif // !ANITOMY_LIB_UTILS_H
