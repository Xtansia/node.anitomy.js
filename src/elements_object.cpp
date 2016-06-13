/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "elements_object.h"
#include "anitomy_object.h"
#include "element_pair_object.h"
#include "util.h"

NAN_MODULE_INIT(ElementsObject::Init) {
  Nan::HandleScope scope;

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(NODE_LOCAL_STRING("Elements"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  Nan::SetPrototypeMethod(tpl, "empty", IsEmpty);
  Nan::SetPrototypeMethod(tpl, "size", Size);
  Nan::SetPrototypeMethod(tpl, "at", At);
  Nan::SetIndexedPropertyHandler(tpl->InstanceTemplate(), IndexGetter, 0, 0, 0, IndexEnumerator);
  Nan::SetPrototypeMethod(tpl, "get", Get);
  Nan::SetPrototypeMethod(tpl, "get_all", GetAll);
  Nan::SetPrototypeMethod(tpl, "clear", Clear);
  Nan::SetPrototypeMethod(tpl, "insert", Insert);
  Nan::SetPrototypeMethod(tpl, "erase", Erase);
  Nan::SetPrototypeMethod(tpl, "count", Count);

  constructor().Reset(tpl->GetFunction());
  Nan::Set(target, NODE_LOCAL_STRING("Elements"), tpl->GetFunction());
}

NAN_METHOD(ElementsObject::New) {
  if (!info.IsConstructCall()) {
    Nan::ThrowError("Cannot call constructor as function, you need to use 'new' keyword");
    return;
  }

  if (info[0]->IsExternal()) {
    v8::Local<v8::External> ext = info[0].As<v8::External>();
    void* ptr = ext->Value();
    ElementsObject* elements = static_cast<ElementsObject*>(ptr);
    elements->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
    return;
  }
  else {
    Nan::ThrowError("Cannot create Elements directly");
  }
}

v8::Local<v8::Value> ElementsObject::New(anitomy::Elements& elements) {
  Nan::EscapableHandleScope scope;

  ElementsObject *wrapped = new ElementsObject(elements);

  v8::Local<v8::Value> ext = Nan::New<v8::External>(wrapped);
  v8::Local<v8::Object> obj = Nan::New(constructor())->NewInstance(1, &ext);

  return scope.Escape(obj);
}

NAN_METHOD(ElementsObject::IsEmpty) {
  NODE_UNWRAP_OBJ(ElementsObject);

  if (info.Length() < 1) { // empty()
    info.GetReturnValue().Set(obj->elements_.empty());
  }
  else { // empty(category)
    NODE_ELEMENT_CATEGORY_ARG(0, category);

    info.GetReturnValue().Set(obj->elements_.empty(category));
  }
}

NAN_METHOD(ElementsObject::Size) {
  NODE_UNWRAP_OBJ(ElementsObject);

  info.GetReturnValue().Set(static_cast<uint32_t>(obj->elements_.size()));
}

NAN_METHOD(ElementsObject::At) {
  if (info.Length() < 1) {
    Nan::ThrowError("index must be given");
    return;
  }
  if (!info[0]->IsInt32() || !info[0]->IsUint32()) {
    Nan::ThrowTypeError("index must be an integer");
    return;
  }

  NODE_UNWRAP_OBJ(ElementsObject);

  size_t index = static_cast<size_t>(info[0]->Uint32Value());

  if (index >= obj->elements_.size()) {
    Nan::ThrowRangeError("index out of range");
    return;
  }

  info.GetReturnValue().Set(ElementPairObject::New(obj->elements_.at(index)));
}

NAN_INDEX_GETTER(ElementsObject::IndexGetter) {
  NODE_UNWRAP_OBJ(ElementsObject);

  if (index >= obj->elements_.size()) {
    Nan::ThrowRangeError("index out of range");
    return;
  }

  info.GetReturnValue().Set(ElementPairObject::New(obj->elements_[static_cast<size_t>(index)]));
}

NAN_INDEX_ENUMERATOR(ElementsObject::IndexEnumerator) {
  NODE_UNWRAP_OBJ(ElementsObject);
  uint32_t size = static_cast<uint32_t>(obj->elements_.size());

  v8::Local<v8::Array> arr = Nan::New<v8::Array>(size);
    
  for (uint32_t i = 0; i < size; ++i) {
    arr->Set(i, Nan::New(i));
  }

  info.GetReturnValue().Set(arr);
}

NAN_METHOD(ElementsObject::Get) {
  NODE_ELEMENT_CATEGORY_ARG(0, category);
  NODE_UNWRAP_OBJ(ElementsObject);

  info.GetReturnValue().Set(NODE_LOCAL_WSTRING(obj->elements_.get(category)));
}

NAN_METHOD(ElementsObject::GetAll) {
  NODE_ELEMENT_CATEGORY_ARG(0, category);
  NODE_UNWRAP_OBJ(ElementsObject);

  std::vector<std::wstring> values = obj->elements_.get_all(category);
  uint32_t size = static_cast<uint32_t>(values.size());

  v8::Local<v8::Array> arr = Nan::New<v8::Array>(size);

  for (uint32_t i = 0; i < size; ++i) {
    arr->Set(i, NODE_LOCAL_WSTRING(values[i]));
  }

  info.GetReturnValue().Set(arr);
}

NAN_METHOD(ElementsObject::Clear) {
  NODE_UNWRAP_OBJ(ElementsObject);

  obj->elements_.clear();
}

NAN_METHOD(ElementsObject::Insert) {
  NODE_ELEMENT_CATEGORY_ARG(0, category);
  NODE_STRING_ARG(1, value);
  NODE_UNWRAP_OBJ(ElementsObject);

  obj->elements_.insert(category, value);
}

NAN_METHOD(ElementsObject::Erase) {
  NODE_ELEMENT_CATEGORY_ARG(0, category);
  NODE_UNWRAP_OBJ(ElementsObject);

  obj->elements_.erase(category);
}

NAN_METHOD(ElementsObject::Count) {
  NODE_ELEMENT_CATEGORY_ARG(0, category);
  NODE_UNWRAP_OBJ(ElementsObject);

  info.GetReturnValue().Set(static_cast<uint32_t>(obj->elements_.count(category)));
}