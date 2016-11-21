/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "utils.h"

#ifdef USE_BOOST_LOCALE_UTF_CONV
  #include <boost/locale/encoding_utf.hpp>
#else
  #include <codecvt>
  #include <locale>
#endif

#include <string>

#ifndef USE_BOOST_LOCALE_UTF_CONV
  static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>
  wstringConverter;
#endif

std::string WstrToStr(const std::wstring &input) {
  #ifdef USE_BOOST_LOCALE_UTF_CONV
  return boost::locale::conv::utf_to_utf<char>(input);
  #else
  return wstringConverter.to_bytes(input);
  #endif
}

std::wstring StrToWstr(const std::string &input) {
  #ifdef USE_BOOST_LOCALE_UTF_CONV
  return boost::locale::conv::utf_to_utf<wchar_t>(input);
  #else
  return wstringConverter.from_bytes(input);
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

bool NodeStringParam(Nan::NAN_METHOD_ARGS_TYPE info, int index,
                     const std::wstring &name, std::wstring &out) {
  if (info.Length() < index + 1 || info[index]->IsUndefined()) {
    Nan::ThrowError(NodeLocalString(name + L" must be provided"));
    return false;
  }

  if (!info[index]->IsString()) {
    Nan::ThrowTypeError(NodeLocalString(name + L" must be a string"));
    return false;
  }

  out = NodeToWstr(info[index]);
  return true;
}

bool NodeCallbackParam(Nan::NAN_METHOD_ARGS_TYPE info, int index,
                       const std::wstring &name, Nan::Callback *&out) {
  if (info.Length() < index + 1 || info[index]->IsUndefined()) {
    Nan::ThrowError(NodeLocalString(name + L" must be provided"));
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
  if (info.Length() < index + 1 || info[index]->IsUndefined()) {
    Nan::ThrowError(NodeLocalString(name + L" must be provided"));
    return false;
  }

  if (!info[index]->IsObject()) {
    Nan::ThrowTypeError(NodeLocalString(name + L" must be an object"));
    return false;
  }

  auto opts = Nan::To<v8::Object>(info[index]).ToLocalChecked();

  IF_OBJ_HAS(opts, L"allowedDelimiters") {
    auto allowedDelimiters = Nan::Get(opts,
                                      NodeLocalString(L"allowedDelimiters")).ToLocalChecked();

    if (allowedDelimiters->IsString()) {
      out.allowed_delimiters = NodeToWstr(allowedDelimiters);
    } else {
      Nan::ThrowTypeError(NodeLocalString(name +
                                          L".allowedDelimiters must be a string"));
      return false;
    }
  }

  IF_OBJ_HAS(opts, L"ignoredStrings") {
    auto ignoredStrings = Nan::Get(opts,
                                   NodeLocalString(L"ignoredStrings")).ToLocalChecked();

    if (ignoredStrings->IsArray()) {
      auto ignoredStringsArray = ignoredStrings.As<v8::Array>();
      v8::Local<v8::Value> elem;

      for (uint32_t i = 0; i < ignoredStringsArray->Length(); ++i) {
        elem = Nan::Get(ignoredStringsArray, i).ToLocalChecked();

        if (elem->IsString()) {
          out.ignored_strings.push_back(NodeToWstr(elem));
        } else {
          Nan::ThrowTypeError(NodeLocalString(name +
                                              L".ignoredStrings must be an array of strings"));
          return false;
        }
      }
    } else {
      Nan::ThrowTypeError(NodeLocalString(name +
                                          L".ignoredStrings must be an array of strings"));
      return false;
    }
  }

#define GET_BOOLEAN_OPTION(jsName, anitomyName)                               \
  IF_OBJ_HAS(opts, WIDE_STRINGIFY(jsName)) {                                  \
    auto jsName = Nan::Get(opts, NodeLocalString(WIDE_STRINGIFY(jsName)))     \
                        .ToLocalChecked();                                    \
    if (jsName->IsBoolean()) {                                                \
      out.anitomyName = Nan::To<bool>(jsName).FromJust();                     \
    } else {                                                                  \
      Nan::ThrowTypeError(NodeLocalString(name                                \
                       + L"." WIDE_STRINGIFY(jsName) L" must be a boolean")); \
      return false;                                                           \
    }                                                                         \
  }

  GET_BOOLEAN_OPTION(parseEpisodeNumber, parse_episode_number);
  GET_BOOLEAN_OPTION(parseEpisodeTitle, parse_episode_title);
  GET_BOOLEAN_OPTION(parseFileExtension, parse_file_extension);
  GET_BOOLEAN_OPTION(parseReleaseGroup, parse_release_group);

  return true;
}
