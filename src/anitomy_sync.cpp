/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "anitomy_sync.h"

#include "anitomy_elements.h"
#include "utils.h"

#include <anitomy/anitomy.h>

NAN_METHOD(ParseSync) {
  std::wstring filename;
  anitomy::Options options;

  if (!NodeStringParam(info, 0, L"filename", filename)) {
    return;
  }

  if (info.Length() > 1
      && !NodeAnitomyOptionsParam(info, 1, L"options", options)) {
    return;
  }

  anitomy::Anitomy anitomy;
  anitomy.options() = options;
  anitomy.Parse(filename);

  v8::Local<v8::Object> elements = AnitomyElements::New(anitomy.elements());

  info.GetReturnValue().Set(elements);
}
