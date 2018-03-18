/*
** Copyright (c) 2016-2018, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once
#ifndef ANITOMY_LIB_ELEMENTS_OBJECT_H
#define ANITOMY_LIB_ELEMENTS_OBJECT_H

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4100 4201 4251)
#endif
#include <nan.h>
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include "utils.h"

#include <anitomy/anitomy.h>
#include <map>

class ElementsObject : public Nan::ObjectWrap {
public:
  static void Init();
  static v8::Local<v8::Object> New(anitomy::Elements &elements);

private:
  static Nan::Persistent<v8::Function> &constructor() {
    static Nan::Persistent<v8::Function> constructor_;
    return constructor_;
  }

  static NAN_METHOD(New);
  static NAN_METHOD(Empty);
  static NAN_METHOD(Size);
  static NAN_METHOD(Count);
  static NAN_METHOD(Get);
  static NAN_METHOD(GetAll);

  static NAN_GETTER(ElementCategoryGetter);

  explicit ElementsObject(anitomy::Elements *elements);
  NAN_DISALLOW_ASSIGN_COPY_MOVE(ElementsObject)
  ~ElementsObject() = default;

  size_t Count(const anitomy::ElementCategory category) const {
    return elements_.count(category);
  }

  bool Empty() const { return elements_.empty(); }

  bool Empty(const anitomy::ElementCategory category) const {
    return elements_.count(category) == 0;
  }

  size_t Size() const { return elements_.size(); }

  std::wstring Get(const anitomy::ElementCategory category) const {
    const auto it = elements_.lower_bound(category);
    return it->first == category ? it->second : L"";
  }

  std::vector<std::wstring>
  GetAll(const anitomy::ElementCategory category) const {
    return MultiMapGetAll(elements_, category);
  }

  std::multimap<anitomy::ElementCategory, std::wstring> elements_;
};

#endif // !ANITOMY_LIB_ELEMENTS_OBJECT_H
