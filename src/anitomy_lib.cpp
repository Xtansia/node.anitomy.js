/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <string>

#include <nan.h>

v8::Local<v8::String> LocalString(const std::string& str) {
  return Nan::New(str).ToLocalChecked();
}

bool StringArg(Nan::NAN_METHOD_ARGS_TYPE info, int index, const std::string& name, std::string& output) {
  if (info.Length() < index + 1 || info[index]->IsUndefined()) {
    Nan::ThrowError(LocalString(name + " must be provided"));
    return false;
  }
  if (!info[index]->IsString()) {
    Nan::ThrowTypeError(LocalString(name + " must be a string"));
    return false;
  }
  output = *Nan::Utf8String(info[index]);
  return true;
}

NAN_METHOD(ParseSync) {
  std::string filename;
  if (!StringArg(info, 0, "filename", filename)) return;
}

NAN_MODULE_INIT(Init) {
  Nan::Export(target, "parseSync", ParseSync);
}

NODE_MODULE(anitomy_lib, Init);