/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once
#ifndef ANITOMY_LIB_ELEMENTS_OBJECT_H
#define ANITOMY_LIB_ELEMENTS_OBJECT_H

#include "nan_nowarn.h"
#include "utils.h"
#include <anitomy/anitomy.h>
#include <map>

class ElementsObject : public Nan::ObjectWrap {
public:
  static void Init();
  static v8::Local<v8::Object> New(const anitomy::Elements &elements);

private:
  static inline Nan::Persistent<v8::Function> &constructor() {
    static Nan::Persistent<v8::Function> constructor_;
    return constructor_;
  }

  static NAN_METHOD(New);
  static NAN_METHOD(Empty);
  static NAN_METHOD(Size);

  static NAN_GETTER(ElementCategoryGetter);

  explicit ElementsObject(anitomy::Elements *elements);
  ~ElementsObject() {}

  inline size_t Count(anitomy::ElementCategory category) const {
    return elements_.count(category);
  }

  inline bool Empty() const {
    return elements_.empty();
  }

  inline size_t Size() const {
    return elements_.size();
  }

  inline std::vector<std::wstring> GetAll(anitomy::ElementCategory category)
  const {
    return MultiMapGetAll(elements_, category);
  }

  std::multimap<anitomy::ElementCategory, std::wstring> elements_;
};

#endif // !ANITOMY_LIB_ELEMENTS_OBJECT_H
