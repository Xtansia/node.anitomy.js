/*
** Copyright (c) 2016-2017, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4100 4201 4251)
#endif
#include <nan.h>
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include "elements_object.h"
#include "parse_worker.h"
#include "utils.h"

#include <anitomy/anitomy.h>
#include <iterator>

inline bool GetOptionsFromObject(const v8::Local<v8::Object> obj,
                                 anitomy::Options &out) {
  return NodeObjectGetIfHas(obj, L"options", L"allowedDelimiters",
                            out.allowed_delimiters) &&
         NodeObjectGetIfHas(obj, L"options", L"ignoredStrings",
                            out.ignored_strings) &&
         NodeObjectGetIfHas(obj, L"options", L"parseEpisodeNumber",
                            out.parse_episode_number) &&
         NodeObjectGetIfHas(obj, L"options", L"parseEpisodeTitle",
                            out.parse_episode_title) &&
         NodeObjectGetIfHas(obj, L"options", L"parseFileExtension",
                            out.parse_file_extension) &&
         NodeObjectGetIfHas(obj, L"options", L"parseReleaseGroup",
                            out.parse_release_group);
}

NAN_METHOD(ParseAsync) {
  std::vector<std::wstring> filenames;
  anitomy::Options options;
  v8::Local<v8::Function> callback;

  auto i = 0;

  if (!NodeParam(info, i++, L"filenames", filenames) || filenames.empty()) {
    return;
  }

  if (info.Length() > 2) {
    v8::Local<v8::Object> optionsObj;

    if (!NodeParam(info, i++, L"options", optionsObj) ||
        !GetOptionsFromObject(optionsObj, options)) {
      return;
    }
  }

  if (!NodeParam(info, i, L"callback", callback)) {
    return;
  }

  Nan::AsyncQueueWorker(
      new ParseWorker(new Nan::Callback(callback), filenames, options));
}

NAN_METHOD(ParseSync) {
  std::vector<std::wstring> filenames;
  anitomy::Options options;

  if (!NodeParam(info, 0, L"filenames", filenames) || filenames.empty()) {
    return;
  }

  if (info.Length() > 1) {
    v8::Local<v8::Object> optionsObj;

    if (!NodeParam(info, 1, L"options", optionsObj) ||
        !GetOptionsFromObject(optionsObj, options)) {
      return;
    }
  }

  anitomy::Anitomy anitomy;
  anitomy.options() = options;

  if (filenames.size() == 1) {
    anitomy.Parse(filenames[0]);
    info.GetReturnValue().Set(ElementsObject::New(anitomy.elements()));
    return;
  }

  const auto elementsArray = Nan::New<v8::Array>();
  uint32_t i = 0;

  for (const auto &filename : filenames) {
    anitomy.Parse(filename);
    Nan::Set(elementsArray, i++, ElementsObject::New(anitomy.elements()));
  }

  info.GetReturnValue().Set(elementsArray);
}

NAN_METHOD(ParseEachAsync) {
  std::vector<std::wstring> filenames;
  anitomy::Options options;
  v8::Local<v8::Function> callback;

  auto i = 0;

  if (!NodeParam(info, i++, L"filenames", filenames) || filenames.empty()) {
    return;
  }

  if (info.Length() > 2) {
    v8::Local<v8::Object> optionsObj;

    if (!NodeParam(info, i++, L"options", optionsObj) ||
        !GetOptionsFromObject(optionsObj, options)) {
      return;
    }
  }

  if (!NodeParam(info, i, L"callback", callback)) {
    return;
  }

  for (const auto &filename : filenames) {
    Nan::AsyncQueueWorker(
        new ParseEachWorker(new Nan::Callback(callback), filename, options));
  }
}

NAN_METHOD(ParseEachSync) {
  std::vector<std::wstring> filenames;
  anitomy::Options options;
  v8::Local<v8::Function> callback;

  auto i = 0;

  if (!NodeParam(info, i++, L"filenames", filenames) || filenames.empty()) {
    return;
  }

  if (info.Length() > 2) {
    v8::Local<v8::Object> optionsObj;

    if (!NodeParam(info, i++, L"options", optionsObj) ||
        !GetOptionsFromObject(optionsObj, options)) {
      return;
    }
  }

  if (!NodeParam(info, i, L"callback", callback)) {
    return;
  }

  anitomy::Anitomy anitomy;
  anitomy.options() = options;

  v8::Local<v8::Value> argv[2];

  for (const auto &filename : filenames) {
    anitomy.Parse(filename);
    argv[0] = WstrToNode(filename);
    argv[1] = ElementsObject::New(anitomy.elements());
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
