/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "utils.h"

#ifdef NODE_ANITOMY_USE_BOOST
  #include <boost/locale/encoding_utf.hpp>
#else
  #include <codecvt>
  #include <locale>
#endif

#ifndef NODE_ANITOMY_USE_BOOST
  typedef std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> wstr_conv;
#endif

std::string WstrToStr(const std::wstring &input) {
#ifdef NODE_ANITOMY_USE_BOOST
  return boost::locale::conv::utf_to_utf<char>(input);
#else
  static wstr_conv conv;
  return conv.to_bytes(input);
#endif
}

std::wstring StrToWstr(const std::string &input) {
#ifdef NODE_ANITOMY_USE_BOOST
  return boost::locale::conv::utf_to_utf<wchar_t>(input);
#else
  static wstr_conv conv;
  return conv.from_bytes(input);
#endif
}

v8::Local<v8::String> NodeLocalString(const std::wstring &str) {
  return Nan::New(WstrToStr(str)).ToLocalChecked();
}

std::wstring NodeToWstr(v8::Local<v8::Value> value) {
  Nan::Utf8String utf8String(value);

  if (utf8String.length() > 0) {
    return StrToWstr(*utf8String);
  } else {
    return L"";
  }
}

bool NodeEnsureParamProvided(Nan::NAN_METHOD_ARGS_TYPE info, int index,
                             const std::wstring &name) {
  if (info.Length() <= index || info[index]->IsUndefined()) {
    Nan::ThrowError(NodeLocalString(name + L" must be provided"));
    return false;
  }

  return true;
}

bool NodeStringOrArrayParam(Nan::NAN_METHOD_ARGS_TYPE info, int index,
                            const std::wstring &name, std::vector<std::wstring> &out) {
  if (!NodeEnsureParamProvided(info, index, name)) {
    return false;
  }

  if (info[index]->IsString()) {
    out.push_back(NodeToWstr(info[index]));
    return true;
  } else if (!info[index]->IsArray()) {
    Nan::ThrowTypeError(NodeLocalString(name +
                                        L" must be a string or an array of strings"));
    return false;
  }

  auto stringArray = info[index].As<v8::Array>();
  v8::Local<v8::Value> elem;

  for (uint32_t i = 0; i < stringArray->Length(); ++i) {
    elem = Nan::Get(stringArray, i).ToLocalChecked();

    if (elem->IsString()) {
      out.push_back(NodeToWstr(elem));
    } else {
      Nan::ThrowTypeError(NodeLocalString(name +
                                          L" must be a string or an array of strings"));
      return false;
    }
  }

  return true;
}

bool NodeCallbackParam(Nan::NAN_METHOD_ARGS_TYPE info, int index,
                       const std::wstring &name, Nan::Callback *&out) {
  if (!NodeEnsureParamProvided(info, index, name)) {
    return false;
  }

  if (!info[index]->IsFunction()) {
    Nan::ThrowTypeError(NodeLocalString(name + L" must be a function"));
    return false;
  }

  out = new Nan::Callback(info[index].As<v8::Function>());
  return true;
}

bool NodeAnitomyOptionsParam(Nan::NAN_METHOD_ARGS_TYPE info, int index,
                             const std::wstring &name, anitomy::Options &out) {
  if (!NodeEnsureParamProvided(info, index, name)) {
    return false;
  }

  if (!info[index]->IsObject()) {
    Nan::ThrowTypeError(NodeLocalString(name + L" must be an object"));
    return false;
  }

  auto opts = Nan::To<v8::Object>(info[index]).ToLocalChecked();

  IF_OBJ_HAS(opts, L"allowedDelimiters") {
    auto allowedDelimiters = Nan::Get(opts,
                                      NodeLocalString(L"allowedDelimiters"))
                             .ToLocalChecked();

    if (allowedDelimiters->IsString()) {
      out.allowed_delimiters = NodeToWstr(allowedDelimiters);
    } else {
      Nan::ThrowTypeError(NodeLocalString(name +
                                          L".allowedDelimiters must be"
                                          L" a string"));
      return false;
    }
  }

  IF_OBJ_HAS(opts, L"ignoredStrings") {
    auto ignoredStrings = Nan::Get(opts,
                                   NodeLocalString(L"ignoredStrings"))
                          .ToLocalChecked();

    if (ignoredStrings->IsArray()) {
      auto ignoredStringsArray = ignoredStrings.As<v8::Array>();
      v8::Local<v8::Value> elem;

      for (uint32_t i = 0; i < ignoredStringsArray->Length(); ++i) {
        elem = Nan::Get(ignoredStringsArray, i).ToLocalChecked();

        if (elem->IsString()) {
          out.ignored_strings.push_back(NodeToWstr(elem));
        } else {
          Nan::ThrowTypeError(NodeLocalString(name +
                                              L".ignoredStrings must be an"
                                              L" array of strings"));
          return false;
        }
      }
    } else {
      Nan::ThrowTypeError(NodeLocalString(name +
                                          L".ignoredStrings must be an array"
                                          L" of strings"));
      return false;
    }
  }

#define GET_BOOLEAN_OPTION(jsName, anitomyName)                               \
  IF_OBJ_HAS(opts, WIDE_STRINGIFY(jsName)) {                                  \
    auto jsName = Nan::Get(opts, NodeLocalString(WIDE_STRINGIFY(jsName)))     \
                  .ToLocalChecked();                                          \
    if (jsName->IsBoolean()) {                                                \
      out.anitomyName = Nan::To<bool>(jsName).FromJust();                     \
    } else {                                                                  \
      Nan::ThrowTypeError(NodeLocalString(name                                \
                                          + L"." WIDE_STRINGIFY(jsName)       \
                                          L" must be a boolean"));          \
      return false;                                                           \
    }                                                                         \
  }

  GET_BOOLEAN_OPTION(parseEpisodeNumber, parse_episode_number);
  GET_BOOLEAN_OPTION(parseEpisodeTitle, parse_episode_title);
  GET_BOOLEAN_OPTION(parseFileExtension, parse_file_extension);
  GET_BOOLEAN_OPTION(parseReleaseGroup, parse_release_group);

  return true;
}
