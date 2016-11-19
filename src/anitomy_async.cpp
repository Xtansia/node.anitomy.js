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

ParseWorker::ParseWorker(Nan::Callback *callback, const std::wstring &filename)
  : Nan::AsyncWorker(callback), filename_(filename) {
}

void ParseWorker::Execute() {
  anitomy::Anitomy anitomy;
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
  Nan::Callback *callback = nullptr;

  if (!NodeStringParam(info, 0, L"filename", filename)
      || !NodeCallbackParam(info, 1, L"callback", callback)) {
    return;
  }

  Nan::AsyncQueueWorker(new ParseWorker(callback, filename));
}
