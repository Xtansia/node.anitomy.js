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

bool GetOptionsFromObject(v8::Local<v8::Object> obj, anitomy::Options &out) {
#define GET_OPTION(jsName, outVar) \
  if (NodeObjectHas(obj, jsName) \
      && !NodeObjectGet(obj, L"options", jsName, outVar)) { \
    return false; \
  }

  GET_OPTION(L"allowedDelimiters", out.allowed_delimiters);
  GET_OPTION(L"ignoredStrings", out.ignored_strings);
  GET_OPTION(L"parseEpisodeNumber", out.parse_episode_number);
  GET_OPTION(L"parseEpisodeTitle", out.parse_episode_title);
  GET_OPTION(L"parseFileExtension", out.parse_file_extension);
  GET_OPTION(L"parseReleaseGroup", out.parse_release_group);

#undef GET_OPTION

  return true;
}

NAN_METHOD(ParseAsync) {
  std::vector<std::wstring> filenames;
  anitomy::Options options;
  v8::Local<v8::Function> callback;

  auto i = 0;

  if (!NodeStringOrArrayParam(info, i++, L"filenames", filenames)
      || filenames.empty()) {
    return;
  }

  if (info.Length() > 2) {
    v8::Local<v8::Object> optionsObj;

    if (!NodeObjectParam(info, i++, L"options", optionsObj)
        || !GetOptionsFromObject(optionsObj, options)) {
      return;
    }
  }

  if (!NodeFunctionParam(info, i, L"callback", callback)) {
    return;
  }

  Nan::AsyncQueueWorker(new ParseWorker(new Nan::Callback(callback), filenames,
                                        options));
}

NAN_METHOD(ParseSync) {
  std::vector<std::wstring> filenames;
  anitomy::Options options;

  if (!NodeStringOrArrayParam(info, 0, L"filenames", filenames)
      || filenames.empty()) {
    return;
  }

  if (info.Length() > 1) {
    v8::Local<v8::Object> optionsObj;

    if (!NodeObjectParam(info, 1, L"options", optionsObj)
        || !GetOptionsFromObject(optionsObj, options)) {
      return;
    }
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

NAN_METHOD(ParseEachAsync) {
  std::vector<std::wstring> filenames;
  anitomy::Options options;
  v8::Local<v8::Function> callback;

  auto i = 0;

  if (!NodeStringOrArrayParam(info, i++, L"filenames", filenames)
      || filenames.empty()) {
    return;
  }

  if (info.Length() > 2) {
    v8::Local<v8::Object> optionsObj;

    if (!NodeObjectParam(info, i++, L"options", optionsObj)
        || !GetOptionsFromObject(optionsObj, options)) {
      return;
    }
  }

  if (!NodeFunctionParam(info, i, L"callback", callback)) {
    return;
  }

  for (const auto &filename : filenames) {
    Nan::AsyncQueueWorker(new ParseEachWorker(new Nan::Callback(callback), filename,
                          options));
  }
}

NAN_METHOD(ParseEachSync) {
  std::vector<std::wstring> filenames;
  anitomy::Options options;
  v8::Local<v8::Function> callback;

  auto i = 0;

  if (!NodeStringOrArrayParam(info, i++, L"filenames", filenames)
      || filenames.empty()) {
    return;
  }

  if (info.Length() > 2) {
    v8::Local<v8::Object> optionsObj;

    if (!NodeObjectParam(info, i++, L"options", optionsObj)
        || !GetOptionsFromObject(optionsObj, options)) {
      return;
    }
  }

  if (!NodeFunctionParam(info, i, L"callback", callback)) {
    return;
  }

  anitomy::Anitomy anitomy;
  anitomy.options() = options;
  anitomy::element_container_t elements;

  v8::Local<v8::Value> argv[2];

  for (const auto &filename : filenames) {
    elements.clear();
    anitomy.Parse(filename);
    std::copy(anitomy.elements().begin(), anitomy.elements().end(),
              std::back_inserter(elements));
    argv[0] = NodeLocalString(filename);
    argv[1] = ElementsObject::New(elements);
    Nan::Call(callback, Nan::GetCurrentContext()->Global(), 2, argv);
  }
}

NAN_MODULE_INIT(Init) {
  ElementsObject::Init();

  Nan::Export(target, "parse", ParseAsync);
  Nan::Export(target, "parseSync", ParseSync);

  Nan::Export(target, "parseEach", ParseEachAsync);
  Nan::Export(target, "parseEachSync", ParseEachSync);
}

NODE_MODULE(anitomy_lib, Init)
