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

std::wstring NodeToWstr(v8::Local<v8::Value> value) {
  Nan::Utf8String utf8String(value);

  if (utf8String.length() > 0) {
    return StrToWstr(*utf8String);
  } else {
    return L"";
  }
}

bool NodeParam(Nan::NAN_METHOD_ARGS_TYPE info, int index,
               const std::wstring &name, std::wstring &out) {
  if (!NodeEnsureParamProvided(info, index, name)) {
    return false;
  }

  if (!info[index]->IsString()) {
    Nan::ThrowTypeError(NodeLocalString(name +
                                        L" must be a string"));
    return false;
  }

  out = NodeToWstr(info[index]);
  return true;
}

bool NodeParam(Nan::NAN_METHOD_ARGS_TYPE info, int index,
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

bool NodeParam(Nan::NAN_METHOD_ARGS_TYPE info, int index,
               const std::wstring &name, v8::Local<v8::Function> &out) {
  if (!NodeEnsureParamProvided(info, index, name)) {
    return false;
  }

  if (!info[index]->IsFunction()) {
    Nan::ThrowTypeError(NodeLocalString(name + L" must be a function"));
    return false;
  }

  out = info[index].As<v8::Function>();
  return true;
}

bool NodeParam(Nan::NAN_METHOD_ARGS_TYPE info, int index,
               const std::wstring &name, v8::Local<v8::Object> &out) {
  if (!NodeEnsureParamProvided(info, index, name)) {
    return false;
  }

  if (!info[index]->IsObject()) {
    Nan::ThrowTypeError(NodeLocalString(name + L" must be an object"));
    return false;
  }

  out = Nan::To<v8::Object>(info[index]).ToLocalChecked();
  return true;
}

bool NodeObjectGet(v8::Local<v8::Object> obj, const std::wstring &objName,
                   const std::wstring &key, std::wstring &out) {
  auto val = Nan::Get(obj, NodeLocalString(key));

  if (val.IsEmpty() || !val.ToLocalChecked()->IsString()) {
    Nan::ThrowTypeError(NodeLocalString(objName + L"." + key +
                                        L" must be a string"));
    return false;
  }

  out = NodeToWstr(val.ToLocalChecked());
  return true;
}

bool NodeObjectGet(v8::Local<v8::Object> obj, const std::wstring &objName,
                   const std::wstring &key, std::vector<std::wstring> &out) {
  auto val = Nan::Get(obj, NodeLocalString(key));

  if (val.IsEmpty() || !val.ToLocalChecked()->IsArray()) {
    Nan::ThrowTypeError(NodeLocalString(objName + L"." + key +
                                        L" must be an array of strings"));
    return false;
  }

  auto arr = val.ToLocalChecked().As<v8::Array>();
  Nan::MaybeLocal<v8::Value> elem;

  for (auto i = 0U; i < arr->Length(); ++i) {
    elem = Nan::Get(arr, i);

    if (elem.IsEmpty() || !elem.ToLocalChecked()->IsString()) {
      Nan::ThrowTypeError(NodeLocalString(objName + L"." + key +
                                          L" must be an array of strings"));
      return false;
    }

    out.push_back(NodeToWstr(elem.ToLocalChecked()));
  }

  return true;
}

bool NodeObjectGet(v8::Local<v8::Object> obj, const std::wstring &objName,
                   const std::wstring &key, bool &out) {
  auto val = Nan::Get(obj, NodeLocalString(key));

  if (val.IsEmpty() || !val.ToLocalChecked()->IsBoolean()) {
    Nan::ThrowTypeError(NodeLocalString(objName + L"." + key +
                                        L" must be a boolean"));
    return false;
  }

  out = Nan::To<bool>(val.ToLocalChecked()).FromMaybe(out);
  return true;
}
