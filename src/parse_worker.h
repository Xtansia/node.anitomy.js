/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once
#ifndef ANITOMY_LIB_PARSE_WORKER_H
#define ANITOMY_LIB_PARSE_WORKER_H

#include "nan_nowarn.h"
#include <anitomy/anitomy.h>

class ParseWorker : public Nan::AsyncWorker {
public:
  ParseWorker(Nan::Callback *callback, const std::vector<std::wstring> &filenames,
              const anitomy::Options &options)
    : Nan::AsyncWorker(callback), filenames_(filenames), options_(options) {}
  ~ParseWorker() {}

  void Execute() override;
  void HandleOKCallback() override;

protected:
  const std::vector<std::wstring> filenames_;
  const anitomy::Options options_;
  std::vector<anitomy::Elements> elements_;
};

class ParseEachWorker : public ParseWorker {
public:
  ParseEachWorker(Nan::Callback *callback, const std::wstring &filename,
                  const anitomy::Options &options)
    : ParseWorker(callback, {
    filename
  }, options) {}

  void HandleOKCallback() override;
};

#endif // !ANITOMY_LIB_PARSE_WORKER_H
