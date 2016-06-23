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

  for (const auto& it : ElementCategoryNames) {
    Nan::SetAccessor(tpl->InstanceTemplate(), NodeLocalString(it.first), ElementCategoryGetter);
  }

  constructor().Reset(tpl->GetFunction());
}

NAN_METHOD(AnitomyElements::New) {
  if (!info.IsConstructCall()) {
    Nan::ThrowError("Cannot call constructor as function, you need to use 'new' keyword");
    return;
  }
  if (!info[0]->IsExternal()) {
    Nan::ThrowError("Cannot create AnitomyElements directly");
    return;
  }

  v8::Local<v8::External> ext = info[0].As<v8::External>();
  AnitomyElements* elements = static_cast<AnitomyElements*>(ext->Value());
  elements->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> AnitomyElements::New(const anitomy::Elements& elements) {
  return New(anitomy::element_container_t(elements.begin(), elements.end()));
}

v8::Local<v8::Object> AnitomyElements::New(const anitomy::element_container_t& elements) {
  Nan::EscapableHandleScope scope;
  
  AnitomyElements* wrapped = new AnitomyElements(elements);
  v8::Local<v8::Value> argv[] = {
    Nan::New<v8::External>(wrapped)
  };
  v8::Local<v8::Object> obj = Nan::New(constructor())->NewInstance(1, argv);

  return scope.Escape(obj);
}

anitomy::element_iterator_t AnitomyElements::Find(anitomy::ElementCategory category) {
  return std::find_if(elements_.begin(), elements_.end(), 
    [category](const anitomy::element_pair_t& pair) {
      return pair.first == category;
    }
  );
}

NAN_GETTER(AnitomyElements::ElementCategoryGetter) {
  auto elemCatIt = ElementCategoryNames.find(StrToWstr(*Nan::Utf8String(property)));
  if (elemCatIt == ElementCategoryNames.end()) {
    return;
  }
  anitomy::ElementCategory category = elemCatIt->second;

  AnitomyElements* obj = ObjectWrap::Unwrap<AnitomyElements>(info.Holder());
  std::vector<std::wstring> values;

  for (const auto& elem : obj->elements_) {
    if (elem.first == category) {
      values.push_back(elem.second);
    }
  }

  if (values.size() == 0) {
    return;
  }
  if (values.size() == 1) {
    info.GetReturnValue().Set(NodeLocalString(values[0]));
    return;
  }

  v8::Local<v8::Array> arr = Nan::New<v8::Array>();
  uint32_t i = 0;

  for (const auto& val : values) {
    Nan::Set(arr, i++, NodeLocalString(val));
  }

  info.GetReturnValue().Set(arr);
}