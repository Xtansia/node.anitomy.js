/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once
#ifndef ANITOMYJS_OBJECTS_ELEMENTPAIR_H
#define ANITOMYJS_OBJECTS_ELEMENTPAIR_H

#include <nan.h>
#include <anitomy/element.h>

#include "elements_obj.h"

namespace objects {
  class Elements;

  class ElementPair : Nan::ObjectWrap {
  public:
    static NAN_MODULE_INIT(Init);

  private:
    ElementPair(const anitomy::element_pair_t& pair) : category_(GetNameForCategory(pair.first)), value_(pair.second) {}
    ~ElementPair() {}

    static NAN_METHOD(New);
    static v8::Local<v8::Value> New(const anitomy::element_pair_t& pair);
    static NAN_GETTER(GetCategory);
    static NAN_GETTER(GetValue);

    static inline Nan::Persistent<v8::Function>& constructor() {
      static Nan::Persistent<v8::Function> constructor_;
      return constructor_;
    }

    const std::wstring category_;
    const std::wstring value_;

    friend class Elements;
  };
}

#endif // !ANITOMYJS_OBJECTS_ELEMENTPAIR_H