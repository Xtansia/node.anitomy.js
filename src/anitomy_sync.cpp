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
#include <iterator>

NAN_METHOD(ParseSync) {
  std::vector<std::wstring> filenames;
  anitomy::Options options;

  if (!NodeStringOrArrayParam(info, 0, L"filenames", filenames) || filenames.empty()) {
    return;
  }

  if (info.Length() > 1
      && !NodeAnitomyOptionsParam(info, 1, L"options", options)) {
    return;
  }

  anitomy::Anitomy anitomy;
  anitomy.options() = options;
  anitomy::element_container_t elements;

  if (filenames.size() == 1) {
    anitomy.Parse(filenames[0]);
    std::copy(anitomy.elements().begin(), anitomy.elements().end(), std::back_inserter(elements));
    info.GetReturnValue().Set(AnitomyElements::New(elements));
    return;
  }

  auto elementsArray = Nan::New<v8::Array>();
  uint32_t i = 0;

  for (const auto &filename : filenames) {
    elements.clear();
    anitomy.Parse(filename);
    std::copy(anitomy.elements().begin(), anitomy.elements().end(), std::back_inserter(elements));
    Nan::Set(elementsArray, i++, AnitomyElements::New(elements));
  }

  info.GetReturnValue().Set(elementsArray);
}
