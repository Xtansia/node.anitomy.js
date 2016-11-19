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

#include <nan.h>
#include <string>

v8::Local<v8::String> NodeLocalString(const std::wstring &str);

bool NodeStringParam(Nan::NAN_METHOD_ARGS_TYPE info, int index,
                     const std::wstring &name, std::wstring &out);
bool NodeCallbackParam(Nan::NAN_METHOD_ARGS_TYPE info, int index,
                       const std::wstring &name, Nan::Callback *&out);

std::string WstrToStr(const std::wstring &input);
std::wstring StrToWstr(const std::string &input);

#endif // !ANITOMY_LIB_UTILS_H