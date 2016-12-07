/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "elements_object.h"

#include "anitomy_element_categories.h"
#include "utils.h"

void ElementsObject::Init() {
  Nan::HandleScope scope;

  auto tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(NodeLocalString(L"AnitomyElements"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "empty", Empty);

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
  auto *elements = static_cast<anitomy::element_container_t *>(ext->Value());
  auto *obj = new ElementsObject(elements);
  obj->Wrap(info.This());

  for (const auto &it : ElementCategoryNames) {
    if (obj->Count(it.second) > 0) {
      Nan::SetAccessor(info.This(), NodeLocalString(it.first),
                       ElementCategoryGetter);
    }
  }

  info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> ElementsObject::New(anitomy::element_container_t
    &elements) {
  Nan::EscapableHandleScope scope;

  v8::Local<v8::Value> argv[] = {
    Nan::New<v8::External>(&elements)
  };

  auto cons = Nan::New(constructor());

  return scope.Escape(Nan::NewInstance(cons, 1, argv).ToLocalChecked());
}

NAN_METHOD(ElementsObject::Empty) {
  auto *obj = Unwrap<ElementsObject>(info.Holder());
  info.GetReturnValue().Set(obj->Empty());
}

NAN_GETTER(ElementsObject::ElementCategoryGetter) {
  auto elemCatIt = ElementCategoryNames.find(NodeToWstr(property));

  if (elemCatIt == ElementCategoryNames.end()) {
    return;
  }

  auto category = elemCatIt->second;
  auto *obj = Unwrap<ElementsObject>(info.Holder());
  auto &values = obj->elements_[category];

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

ElementsObject::ElementsObject(anitomy::element_container_t *elements) {
  for (const auto &it : *elements) {
    elements_[it.first].push_back(it.second);
  }
}

std::size_t ElementsObject::Count(anitomy::ElementCategory category) const {
  return elements_[category].size();
}

bool ElementsObject::Empty() const {
  for (uint32_t i = 0; i < ELEMENT_CATEGORY_COUNT; ++i) {
    if (Count(anitomy::ElementCategory(i)) > 0) {
      return false;
    }
  }

  return true;
}
