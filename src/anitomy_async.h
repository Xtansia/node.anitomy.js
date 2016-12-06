/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once
#ifndef ANITOMY_LIB_ANITOMY_ASYNC_H
#define ANITOMY_LIB_ANITOMY_ASYNC_H

#include "nan_nowarn.h"
#include <anitomy/anitomy.h>

class ParseWorker : public Nan::AsyncWorker {
public:
  ParseWorker(Nan::Callback *callback, const std::vector<std::wstring> &filenames,
              const anitomy::Options &options);
  ~ParseWorker() {}

  void Execute();
  void HandleOKCallback();

private:
  const std::vector<std::wstring> filenames_;
  const anitomy::Options options_;
  std::vector<anitomy::element_container_t> elements_;
};

NAN_METHOD(ParseAsync);

#endif // !ANITOMY_LIB_ANITOMY_ASYNC_H
