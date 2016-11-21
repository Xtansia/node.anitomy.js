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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <nan.h>
#pragma GCC diagnostic pop

#include <anitomy/options.h>
#include <string>

#define CONCAT_INTERNAL(X, Y) X##Y
#define CONCAT(X, Y) CONCAT_INTERNAL(X, Y)
#define STRINGIFY_INTERNAL(X) #X
#define STRINGIFY(X) STRINGIFY_INTERNAL(X)
#define WIDE_STRINGIFY(X) CONCAT(L, STRINGIFY(X))
#define IF_OBJ_HAS(obj, key) \
  if (Nan::Has(obj, NodeLocalString(key)).FromMaybe(false))

std::string WstrToStr(const std::wstring &input);
std::wstring StrToWstr(const std::string &input);

v8::Local<v8::String> NodeLocalString(const std::wstring &str);
std::wstring NodeToWstr(v8::Local<v8::Value> value);

bool NodeStringParam(Nan::NAN_METHOD_ARGS_TYPE info, int index,
                     const std::wstring &name, std::wstring &out);
bool NodeCallbackParam(Nan::NAN_METHOD_ARGS_TYPE info, int index,
                       const std::wstring &name, Nan::Callback *&out);
bool NodeAnitomyOptionsParam(Nan::NAN_METHOD_ARGS_TYPE info, int index,
                             const std::wstring &name, anitomy::Options &out);

#endif // !ANITOMY_LIB_UTILS_H
