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

class AnitomyElements : public Nan::ObjectWrap {
public:
  static void Init();
  static v8::Local<v8::Object> New(const anitomy::Elements& elements);
  static v8::Local<v8::Object> New(const anitomy::element_container_t& elements);

private:
  AnitomyElements(const anitomy::element_container_t& elements) : elements_(elements) {};
  ~AnitomyElements() {};

  static NAN_METHOD(New);
  static NAN_GETTER(ElementCategoryGetter);

  anitomy::element_iterator_t Find(anitomy::ElementCategory category);

  anitomy::element_container_t elements_;

  static inline Nan::Persistent<v8::Function>& constructor() {
    static Nan::Persistent<v8::Function> constructor_;
    return constructor_;
  }
};

#endif // !ANITOMY_LIB_ANITOMY_ELEMENTS_H



