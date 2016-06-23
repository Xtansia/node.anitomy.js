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

#include <anitomy/anitomy.h>
#include <nan.h>

class ParseWorker : public Nan::AsyncWorker {
public:
  ParseWorker(Nan::Callback* callback, const std::wstring& filename);
  ~ParseWorker() {}

  void Execute();
  void HandleOKCallback();

private:
  const std::wstring filename_;
  anitomy::element_container_t elements_;
};

NAN_METHOD(ParseAsync);

#endif // !ANITOMY_LIB_ANITOMY_ASYNC_H