/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once
#ifndef ANITOMYJS_ELEMENTS_OBJECT_H
#define ANITOMYJS_ELEMENTS_OBJECT_H

#include <nan.h>
#include <anitomy/element.h>
#include "element_categories.h"

class ElementsObject : public Nan::ObjectWrap {
public:
  static void Init();

private:
  ElementsObject(anitomy::Elements& elements) : elements_(elements) {};
  ~ElementsObject() {};

  static NAN_METHOD(New);
  static v8::Local<v8::Value> New(anitomy::Elements& elements);
  static NAN_METHOD(IsEmpty);
  static NAN_METHOD(Size);
  static NAN_METHOD(At);
  static NAN_INDEX_GETTER(IndexGetter);
  static NAN_INDEX_ENUMERATOR(IndexEnumerator);
  static NAN_METHOD(Get);
  static NAN_METHOD(GetAll);
  static NAN_METHOD(Clear);
  static NAN_METHOD(Insert);
  static NAN_METHOD(Erase);
  static NAN_METHOD(Count);

  static inline Nan::Persistent<v8::Function>& constructor() {
    static Nan::Persistent<v8::Function> constructor_;
    return constructor_;
  }

  anitomy::Elements& elements_;

  friend class AnitomyObject;
};

#define NODE_ELEMENT_CATEGORY_ARG(i, name) \
  if (info.Length() < i + 1) { \
    Nan::ThrowError(#name " must be given"); \
    return; \
  } \
  if (!info[i]->IsString()) { \
    Nan::ThrowTypeError(#name " must be a string"); \
    return; \
  } \
  auto elemCatIt_##i##name = ElementCategoryNames.find(StrToWstr(*Nan::Utf8String(info[i]))); \
  if (elemCatIt_##i##name == ElementCategoryNames.end()) { \
    Nan::ThrowTypeError(#name " is not a valid ElementCategory"); \
    return; \
  } \
  anitomy::ElementCategory name = elemCatIt_##i##name->second;

#endif // !ANITOMYJS_ELEMENTS_OBJECT_H
