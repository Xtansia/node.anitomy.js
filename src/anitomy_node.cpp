/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <nan.h>
#include "anitomy_object.h"
#include "elements_object.h"
#include "element_pair_object.h"
#include "options_object.h"

void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  AnitomyObject::Init(module);
  ElementsObject::Init();
  ElementPairObject::Init();
  OptionsObject::Init();
}

NODE_MODULE(anitomy_lib, Init)