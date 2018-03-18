/*
** Copyright (c) 2016-2018, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4100 4201 4251)
#endif
#include <nan.h>
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include <anitomy/anitomy.h>
#include <utility>

class ParseWorker : public Nan::AsyncWorker {
public:
  ParseWorker(Nan::Callback *callback, std::vector<std::wstring> filenames,
              anitomy::Options options)
      : AsyncWorker(callback, "anitomyjs:ParseWorker"),
        filenames_(std::move(filenames)), options_(std::move(options)) {}
  ~ParseWorker() = default;

  void Execute() override;
  void HandleOKCallback() override;

protected:
  const std::vector<std::wstring> filenames_;
  const anitomy::Options options_;
  std::vector<anitomy::Elements> elements_;

private:
  NAN_DISALLOW_ASSIGN_COPY_MOVE(ParseWorker)
};

class ParseEachWorker : public ParseWorker {
public:
  ParseEachWorker(Nan::Callback *callback, const std::wstring &filename,
                  const anitomy::Options &options)
      : ParseWorker(callback, {filename}, options) {}

  void HandleOKCallback() override;
};