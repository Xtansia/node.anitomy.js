/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once
#ifndef ANITOMY_LIB_ANITOMY_ELEMENTS_H
#define ANITOMY_LIB_ANITOMY_ELEMENTS_H

#include <anitomy/anitomy.h>
#include <nan.h>

#define ELEMENT_CATEGORY_COUNT anitomy::ElementCategory::kElementIterateLast + 1

class AnitomyElements : public Nan::ObjectWrap {
public:
  static void Init();
  static v8::Local<v8::Object> New(const anitomy::Elements &elements);
  static v8::Local<v8::Object> New(const anitomy::element_container_t &elements);

private:
  static inline Nan::Persistent<v8::Function> &constructor() {
    static Nan::Persistent<v8::Function> constructor_;
    return constructor_;
  }

  static NAN_METHOD(New);
  static NAN_METHOD(Empty);
  static NAN_GETTER(ElementCategoryGetter);

  AnitomyElements(const anitomy::element_container_t &elements);
  ~AnitomyElements() {}

  std::size_t Count(anitomy::ElementCategory category);
  bool Empty();

  std::vector<std::wstring> elements_[ELEMENT_CATEGORY_COUNT];
};

#endif // !ANITOMY_LIB_ANITOMY_ELEMENTS_H
