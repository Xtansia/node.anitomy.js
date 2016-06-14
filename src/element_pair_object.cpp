/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "element_pair_object.h"
#include "util.h"

void ElementPairObject::Init() {
  Nan::HandleScope scope;

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(NODE_LOCAL_STRING("ElementPair"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetAccessor(tpl->InstanceTemplate(), NODE_LOCAL_STRING("category"), GetCategory, READ_ONLY_SETTER);
  Nan::SetAccessor(tpl->InstanceTemplate(), NODE_LOCAL_STRING("value"), GetValue, READ_ONLY_SETTER);

  constructor().Reset(tpl->GetFunction());
}

NAN_METHOD(ElementPairObject::New) {
  if (!info.IsConstructCall()) {
    Nan::ThrowError("Cannot call constructor as function, you need to use 'new' keyword");
    return;
  }

  if (info[0]->IsExternal()) {
    v8::Local<v8::External> ext = info[0].As<v8::External>();
    void* ptr = ext->Value();
    ElementPairObject *elementPair = static_cast<ElementPairObject *>(ptr);
    elementPair->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
    return;
  }
  else {
    Nan::ThrowError("Cannot create ElementPair directly");
  }
}

v8::Local<v8::Value> ElementPairObject::New(const anitomy::element_pair_t& pair) {
  Nan::EscapableHandleScope scope;

  ElementPairObject* wrapped = new ElementPairObject(pair);

  v8::Local<v8::Value> ext = Nan::New<v8::External>(wrapped);
  v8::Local<v8::Object> obj = Nan::New(constructor())->NewInstance(1, &ext);
    
  return scope.Escape(obj);
}

NAN_GETTER(ElementPairObject::GetCategory) {
  NODE_UNWRAP_OBJ(ElementPairObject);
  info.GetReturnValue().Set(NODE_LOCAL_WSTRING(obj->category_));
}

NAN_GETTER(ElementPairObject::GetValue) {
  NODE_UNWRAP_OBJ(ElementPairObject);
  info.GetReturnValue().Set(NODE_LOCAL_WSTRING(obj->value_));
}