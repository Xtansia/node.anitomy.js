/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "anitomy_elements.h"

#include "anitomy_element_categories.h"
#include "utils.h"

void AnitomyElements::Init() {
  Nan::HandleScope scope;

  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("AnitomyElements").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "empty", Empty);

  constructor().Reset(tpl->GetFunction());
}

NAN_METHOD(AnitomyElements::New) {
  if (!info.IsConstructCall()) {
    Nan::ThrowError(
      "Cannot call constructor as function, you need to use 'new' keyword");
    return;
  }

  if (!info[0]->IsExternal()) {
    Nan::ThrowError("Cannot create AnitomyElements directly");
    return;
  }

  v8::Local<v8::External> ext = info[0].As<v8::External>();
  AnitomyElements *elements = static_cast<AnitomyElements *>(ext->Value());
  elements->Wrap(info.This());

  for (const auto &it : ElementCategoryNames) {
    if (elements->Count(it.second) > 0) {
      Nan::SetAccessor(info.This(), NodeLocalString(it.first),
                       ElementCategoryGetter);
    }
  }

  info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> AnitomyElements::New(const anitomy::Elements &elements) {
  return New(anitomy::element_container_t(elements.begin(), elements.end()));
}

v8::Local<v8::Object> AnitomyElements::New(const anitomy::element_container_t
    &elements) {
  Nan::EscapableHandleScope scope;

  AnitomyElements *wrapped = new AnitomyElements(elements);
  v8::Local<v8::Value> argv[] = {
    Nan::New<v8::External>(wrapped)
  };
  v8::Local<v8::Object> obj = Nan::New(constructor())->NewInstance(1, argv);

  return scope.Escape(obj);
}

NAN_METHOD(AnitomyElements::Empty) {
  AnitomyElements *obj = ObjectWrap::Unwrap<AnitomyElements>(info.Holder());
  info.GetReturnValue().Set(obj->Empty());
}

NAN_GETTER(AnitomyElements::ElementCategoryGetter) {
  auto elemCatIt = ElementCategoryNames.find(NodeToWstr(property));

  if (elemCatIt == ElementCategoryNames.end()) {
    return;
  }

  anitomy::ElementCategory category = elemCatIt->second;
  AnitomyElements *obj = ObjectWrap::Unwrap<AnitomyElements>(info.Holder());
  std::vector<std::wstring> &values = obj->elements_[category];

  if (values.size() == 0) {
    return;
  } else if (values.size() == 1) {
    info.GetReturnValue().Set(NodeLocalString(values[0]));
    return;
  }

  v8::Local<v8::Array> arr = Nan::New<v8::Array>();
  uint32_t i = 0;

  for (const auto &val : values) {
    Nan::Set(arr, i++, NodeLocalString(val));
  }

  info.GetReturnValue().Set(arr);
}

AnitomyElements::AnitomyElements(const anitomy::element_container_t &elements) {
  for (const auto &it : elements) {
    elements_[it.first].push_back(it.second);
  }
}

std::size_t AnitomyElements::Count(anitomy::ElementCategory category) {
  return elements_[category].size();
}

bool AnitomyElements::Empty() {
  for (uint32_t i = 0; i < ELEMENT_CATEGORY_COUNT; ++i) {
    if (Count(anitomy::ElementCategory(i)) > 0) {
      return false;
    }
  }

  return true;
}
