/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "elements_object.h"

#include "element_categories.h"
#include "utils.h"

void ElementsObject::Init() {
  Nan::HandleScope scope;

  auto tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(NodeLocalString(L"AnitomyElements"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "empty", Empty);
  Nan::SetPrototypeMethod(tpl, "size", Size);

  constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(ElementsObject::New) {
  if (!info.IsConstructCall()) {
    Nan::ThrowError(
      "Cannot call constructor as function, you need to use 'new' keyword");
    return;
  }

  if (!info[0]->IsExternal()) {
    Nan::ThrowError("Cannot create AnitomyElements directly");
    return;
  }

  auto ext = info[0].As<v8::External>();
  auto *elements = static_cast<anitomy::Elements *>(ext->Value());
  auto *obj = new ElementsObject(elements);
  obj->Wrap(info.This());

  for (const auto &category : ElementCategories) {
    if (!obj->Empty(category)) {
      Nan::SetAccessor(info.This(), NodeLocalString(GetName(category)),
                       ElementCategoryGetter);
    }
  }

  info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> ElementsObject::New(const anitomy::Elements &elements) {
  Nan::EscapableHandleScope scope;

  auto *elems = new anitomy::Elements(elements);

  v8::Local<v8::Value> argv[] = {
    Nan::New<v8::External>(elems)
  };

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
  info.GetReturnValue().Set(obj->Empty());
}

NAN_METHOD(ElementsObject::Size) {
  auto *obj = Unwrap<ElementsObject>(info.Holder());
  info.GetReturnValue().Set(static_cast<uint32_t>(obj->Size()));
}

NAN_GETTER(ElementsObject::ElementCategoryGetter) {
  auto category = GetElementCategory(NodeToWstr(property));

  if (category == NoSuchElementCategory) {
    return;
  }

  auto *obj = Unwrap<ElementsObject>(info.Holder());
  auto values = obj->GetAll(category);

  if (values.size() == 0) {
    return;
  } else if (values.size() == 1) {
    info.GetReturnValue().Set(NodeLocalString(values[0]));
    return;
  }

  auto arr = Nan::New<v8::Array>();
  uint32_t i = 0;

  for (const auto &val : values) {
    Nan::Set(arr, i++, NodeLocalString(val));
  }

  info.GetReturnValue().Set(arr);
}

size_t ElementsObject::Count(anitomy::ElementCategory category) const {
  return elements_.count(category);
}

bool ElementsObject::Empty() const {
  return elements_.empty();
}

size_t ElementsObject::Size() const {
  return elements_.size();
}

std::vector<std::wstring> ElementsObject::GetAll(anitomy::ElementCategory
    category) const {
  return MultiMapGetAll(elements_, category);
}
