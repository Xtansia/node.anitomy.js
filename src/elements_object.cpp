/*
** Copyright (c) 2016-2017, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "elements_object.h"

#include "element_categories.h"

void ElementsObject::Init() {
  Nan::HandleScope scope;

  auto tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(WstrToNode(L"AnitomyElements"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "empty", Empty);
  Nan::SetPrototypeMethod(tpl, "size", Size);
  Nan::SetPrototypeMethod(tpl, "count", Count);
  Nan::SetPrototypeMethod(tpl, "get", Get);
  Nan::SetPrototypeMethod(tpl, "getAll", GetAll);

  constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(ElementsObject::New) {
  if (!info.IsConstructCall()) {
    NodeThrowError(
        L"Cannot call constructor as function, you need to use 'new' keyword");
    return;
  }

  if (!info[0]->IsExternal()) {
    NodeThrowError(L"Cannot create AnitomyElements directly");
    return;
  }

  auto ext = info[0].As<v8::External>();
  auto *elements = static_cast<anitomy::Elements *>(ext->Value());
  auto *obj = new ElementsObject(elements);
  obj->Wrap(info.This());

  for (const auto &category : ElementCategories) {
    if (!obj->Empty(category)) {
      Nan::SetAccessor(info.This(), WstrToNode(GetName(category)),
                       ElementCategoryGetter);
    }
  }

  info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> ElementsObject::New(const anitomy::Elements &elements) {
  Nan::EscapableHandleScope scope;

  auto *elems = new anitomy::Elements(elements);

  v8::Local<v8::Value> argv[] = {Nan::New<v8::External>(elems)};

  auto cons = Nan::New(constructor());
  auto instance = Nan::NewInstance(cons, 1, argv).ToLocalChecked();

  delete elems;

  return scope.Escape(instance);
}

ElementsObject::ElementsObject(anitomy::Elements *elements) {
  for (const auto &it : *elements) {
    elements_.insert(it);
  }
}

NAN_METHOD(ElementsObject::Empty) {
  auto *obj = Unwrap<ElementsObject>(info.Holder());

  if (info.Length() == 0) {
    info.GetReturnValue().Set(obj->Empty());
    return;
  }

  anitomy::ElementCategory category;

  if (!NodeParam(info, 0, L"category", category)) {
    return;
  }

  info.GetReturnValue().Set(obj->Empty(category));
}

NAN_METHOD(ElementsObject::Size) {
  auto *obj = Unwrap<ElementsObject>(info.Holder());
  info.GetReturnValue().Set(static_cast<uint32_t>(obj->Size()));
}

NAN_METHOD(ElementsObject::Count) {
  auto *obj = Unwrap<ElementsObject>(info.Holder());
  anitomy::ElementCategory category;

  if (!NodeParam(info, 0, L"category", category)) {
    return;
  }

  info.GetReturnValue().Set(static_cast<uint32_t>(obj->Count(category)));
}

NAN_METHOD(ElementsObject::Get) {
  auto *obj = Unwrap<ElementsObject>(info.Holder());
  anitomy::ElementCategory category;

  if (!NodeParam(info, 0, L"category", category)) {
    return;
  }

  info.GetReturnValue().Set(WstrToNode(obj->Get(category)));
}

NAN_METHOD(ElementsObject::GetAll) {
  auto *obj = Unwrap<ElementsObject>(info.Holder());
  anitomy::ElementCategory category;

  if (!NodeParam(info, 0, L"category", category)) {
    return;
  }

  auto values = obj->GetAll(category);

  auto arr = Nan::New<v8::Array>();
  uint32_t i = 0;

  for (const auto &val : values) {
    Nan::Set(arr, i++, WstrToNode(val));
  }

  info.GetReturnValue().Set(arr);
}

NAN_GETTER(ElementsObject::ElementCategoryGetter) {
  auto category = GetElementCategory(NodeToWstr(property));

  if (category == anitomy::kElementUnknown) {
    return;
  }

  auto *obj = Unwrap<ElementsObject>(info.Holder());
  auto values = obj->GetAll(category);

  if (values.size() == 0) {
    return;
  } else if (values.size() == 1) {
    info.GetReturnValue().Set(WstrToNode(values[0]));
    return;
  }

  auto arr = Nan::New<v8::Array>();
  uint32_t i = 0;

  for (const auto &val : values) {
    Nan::Set(arr, i++, WstrToNode(val));
  }

  info.GetReturnValue().Set(arr);
}
