/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "anitomy_async.h"

#include "anitomy_elements.h"
#include "utils.h"
#include <iterator>

ParseWorker::ParseWorker(Nan::Callback *callback,
                         const std::vector<std::wstring> &filenames,
                         const anitomy::Options &options)
  : Nan::AsyncWorker(callback), filenames_(filenames), options_(options) {}

void ParseWorker::Execute() {
  anitomy::Anitomy anitomy;
  anitomy.options() = options_;
  anitomy::element_container_t elements;

  for (const auto &filename : filenames_) {
    elements.clear();
    anitomy.Parse(filename);
    std::copy(anitomy.elements().begin(), anitomy.elements().end(),
              std::back_inserter(elements));
    elements_.push_back(elements);
  }
}

void ParseWorker::HandleOKCallback() {
  Nan::HandleScope scope;

  v8::Local<v8::Value> argv[1];

  if (elements_.size() == 1) {
    argv[0] = AnitomyElements::New(elements_[0]);
  } else {
    auto elementsArray = Nan::New<v8::Array>();

    for (uint32_t i = 0; i < elements_.size(); ++i) {
      Nan::Set(elementsArray, i, AnitomyElements::New(elements_[i]));
    }

    argv[0] = elementsArray;
  }

  callback->Call(1, argv);
}

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
