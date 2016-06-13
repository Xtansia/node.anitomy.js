/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once
#ifndef ANITOMYJS_OPTIONS_OBJECT_H
#define ANITOMYJS_OPTIONS_OBJECT_H

#include <nan.h>
#include <anitomy/options.h>

#include "util.h"

class OptionsObject : public Nan::ObjectWrap {
public:
  static NAN_MODULE_INIT(Init);

private:
  OptionsObject(anitomy::Options& options) : options_(options) {};
  ~OptionsObject() {};

  static NAN_METHOD(New);
  static v8::Local<v8::Value> New(anitomy::Options& options);
  NODE_GETTER_AND_SETTER(AllowedDelimiters);
  NODE_GETTER_AND_SETTER(IgnoredStrings);
  NODE_GETTER_AND_SETTER(ParseEpisodeNumber);
  NODE_GETTER_AND_SETTER(ParseEpisodeTitle);
  NODE_GETTER_AND_SETTER(ParseFileExtension);
  NODE_GETTER_AND_SETTER(ParseReleaseGroup);

  static inline Nan::Persistent<v8::Function>& constructor() {
    static Nan::Persistent<v8::Function> constructor_;
    return constructor_;
  }

  anitomy::Options& options_;

  friend class AnitomyObject;
};

#endif // !ANITOMYJS_OPTIONS_OBJECT_H