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

ParseWorker::ParseWorker(Nan::Callback *callback, const std::wstring &filename,
                         const anitomy::Options &options)
  : Nan::AsyncWorker(callback), filename_(filename), options_(options) {
}

void ParseWorker::Execute() {
  anitomy::Anitomy anitomy;
  anitomy.options() = options_;
  anitomy.Parse(filename_);
  elements_ = anitomy::element_container_t(anitomy.elements().begin(),
              anitomy.elements().end());
}

void ParseWorker::HandleOKCallback() {
  Nan::HandleScope scope;

  v8::Local<v8::Value> argv[] = {
    AnitomyElements::New(elements_)
  };

  callback->Call(1, argv);
}

NAN_METHOD(ParseAsync) {
  std::wstring filename;
  anitomy::Options options;
  Nan::Callback *callback = nullptr;

  int i = 0;

  if (!NodeStringParam(info, i++, L"filename", filename)) {
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

  Nan::AsyncQueueWorker(new ParseWorker(callback, filename, options));
}
