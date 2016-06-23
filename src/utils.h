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

v8::Local<v8::String> NodeLocalString(const std::string& str);
bool NodeStringParam(Nan::NAN_METHOD_ARGS_TYPE info, int index, const std::string& name, std::string& out);

#endif // !ANITOMY_LIB_UTILS_H