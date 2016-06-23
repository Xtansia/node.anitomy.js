/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "./utils.h"

v8::Local<v8::String> LocalString(const std::string& str) {
  return Nan::New(str).ToLocalChecked();
}

bool StringParam(Nan::NAN_METHOD_ARGS_TYPE info, int index, const std::string& name, std::string& out) {
  if (info.Length() < index + 1 || info[index]->IsUndefined()) {
    Nan::ThrowError(LocalString(name + " must be provided"));
    return false;
  }
  if (!info[index]->IsString()) {
    Nan::ThrowTypeError(LocalString(name + " must be a string"));
    return false;
  }
  out = *Nan::Utf8String(info[index]);
  return true;
}
