/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "anitomy_object.h"
#include "elements_object.h"
#include "options_object.h"
#include "util.h"

void AnitomyObject::Init(v8::Local<v8::Object> module) {
  Nan::HandleScope scope;

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(NODE_LOCAL_STRING("Anitomy"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeMethod(tpl, "parse", Parse);
  Nan::SetPrototypeMethod(tpl, "elements", GetElements);
  Nan::SetPrototypeMethod(tpl, "options", GetOptions);
  Nan::SetPrototypeMethod(tpl, "tokens", GetTokens);

  constructor().Reset(tpl->GetFunction());
  Nan::Set(module, NODE_LOCAL_STRING("exports"), tpl->GetFunction());
}

NAN_METHOD(AnitomyObject::New) {
  if (!info.IsConstructCall()) {
    Nan::ThrowError("Cannot call constructor as function, you need to use 'new' keyword");
    return;
  }
  if (info.Length() != 0) {
    Nan::ThrowError("Anitomy constructor doesn't take any arguments");
    return;
  }

  AnitomyObject* wrapped = new AnitomyObject();

  v8::Local<v8::Value> elements = ElementsObject::New(wrapped->anitomy_.elements());
  v8::Local<v8::Value> options = OptionsObject::New(wrapped->anitomy_.options());

  info.This()->SetHiddenValue(NODE_LOCAL_STRING("elements_"), elements);
  info.This()->SetHiddenValue(NODE_LOCAL_STRING("options_"), options);

  wrapped->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

NAN_METHOD(AnitomyObject::Parse) {
  NODE_STRING_ARG(0, filename);
  NODE_UNWRAP_OBJ(AnitomyObject);

  info.GetReturnValue().Set(obj->anitomy_.Parse(filename));
}

NAN_METHOD(AnitomyObject::GetElements) {
  info.GetReturnValue().Set(info.This()->GetHiddenValue(NODE_LOCAL_STRING("elements_")));
}

NAN_METHOD(AnitomyObject::GetOptions) {
  info.GetReturnValue().Set(info.This()->GetHiddenValue(NODE_LOCAL_STRING("options_")));
}

NAN_METHOD(AnitomyObject::GetTokens) {
  // TODO: Get tokens
  info.GetReturnValue().SetNull();
}