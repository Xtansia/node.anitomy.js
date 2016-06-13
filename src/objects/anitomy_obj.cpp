/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "anitomy_obj.h"
#include "elements_obj.h"
#include "../util.h"

namespace objects {
  NAN_MODULE_INIT(Anitomy::Init) {
    Nan::HandleScope scope;

    // Prepare constructor template
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(LOCAL_STRING("Anitomy"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "parse", Parse);
    Nan::SetPrototypeMethod(tpl, "elements", GetElements);
    Nan::SetPrototypeMethod(tpl, "options", GetOptions);
    Nan::SetPrototypeMethod(tpl, "tokens", GetTokens);

    constructor().Reset(tpl->GetFunction());
    Nan::Set(target, LOCAL_STRING("Anitomy"), tpl->GetFunction());
  }

  NAN_METHOD(Anitomy::New) {
    if (!info.IsConstructCall()) {
      Nan::ThrowError("Cannot call constructor as function, you need to use 'new' keyword");
      return;
    }
    if (info.Length() != 0) {
      Nan::ThrowError("Anitomy constructor doesn't take any arguments");
      return;
    }

    Anitomy* wrapped = new Anitomy();
    v8::Local<v8::Value> elements = Elements::New(wrapped->anitomy_.elements());
    info.This()->SetHiddenValue(LOCAL_STRING("elements_"), elements);
    wrapped->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  }

  NAN_METHOD(Anitomy::Parse) {
    STRING_ARG(0, filename);
    UNWRAP_OBJ(Anitomy);

    info.GetReturnValue().Set(obj->anitomy_.Parse(filename));
  }

  NAN_METHOD(Anitomy::GetElements) {
    info.GetReturnValue().Set(info.This()->GetHiddenValue(LOCAL_STRING("elements_")));
  }

  NAN_METHOD(Anitomy::GetOptions) {
    // TODO: Get options
    info.GetReturnValue().SetNull();
  }

  NAN_METHOD(Anitomy::GetTokens) {
    // TODO: Get tokens
    info.GetReturnValue().SetNull();
  }
}