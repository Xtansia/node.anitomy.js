/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "nan_nowarn.h"
#include "parse_worker.h"
#include "elements_object.h"
#include "utils.h"

#include <anitomy/anitomy.h>
#include <iterator>

NAN_METHOD(ParseAsync) {
  std::vector<std::wstring> filenames;
  anitomy::Options options;
  Nan::Callback *callback = nullptr;

  auto i = 0;

  if (!NodeStringOrArrayParam(info, i++, L"filenames", filenames)
      || filenames.empty()) {
    return;
  }

  if (info.Length() > 2) {
    if (!NodeAnitomyOptionsParam(info, i++, L"options", options)) {
      return;
    }
  }

  if (!NodeCallbackParam(info, i, L"callback", callback)) {
    return;
  }

  Nan::AsyncQueueWorker(new ParseWorker(callback, filenames, options));
}

NAN_METHOD(ParseSync) {
  std::vector<std::wstring> filenames;
  anitomy::Options options;

  if (!NodeStringOrArrayParam(info, 0, L"filenames", filenames)
      || filenames.empty()) {
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
    std::copy(anitomy.elements().begin(), anitomy.elements().end(),
              std::back_inserter(elements));
    info.GetReturnValue().Set(ElementsObject::New(elements));
    return;
  }

  auto elementsArray = Nan::New<v8::Array>();
  uint32_t i = 0;

  for (const auto &filename : filenames) {
    elements.clear();
    anitomy.Parse(filename);
    std::copy(anitomy.elements().begin(), anitomy.elements().end(),
              std::back_inserter(elements));
    Nan::Set(elementsArray, i++, ElementsObject::New(elements));
  }

  info.GetReturnValue().Set(elementsArray);
}


NAN_MODULE_INIT(Init) {
  ElementsObject::Init();
  Nan::Export(target, "parse", ParseAsync);
  Nan::Export(target, "parseSync", ParseSync);
}

NODE_MODULE(anitomy_lib, Init)
