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

  if (!NodeStringParam(info, 0, L"filename", filename)) {
    return;
  }

  anitomy::Anitomy anitomy;
  anitomy.Parse(filename);

  v8::Local<v8::Object> elements = AnitomyElements::New(anitomy.elements());

  info.GetReturnValue().Set(elements);
}
