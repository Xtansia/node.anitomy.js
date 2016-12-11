/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "utils.h"

#include "element_categories.h"

bool NodeParam(Nan::NAN_METHOD_ARGS_TYPE info, int index,
               const std::wstring &name, std::wstring &out) {
  if (!NodeEnsureParamProvided(info, index, name)) {
    return false;
  }

  if (!info[index]->IsString()) {
    NodeThrowTypeError(name + L" must be a string");
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
    NodeThrowTypeError(name + L" must be a string or an array of strings");
    return false;
  }

  auto stringArray = info[index].As<v8::Array>();
  v8::Local<v8::Value> elem;

  for (uint32_t i = 0; i < stringArray->Length(); ++i) {
    elem = Nan::Get(stringArray, i).ToLocalChecked();

    if (elem->IsString()) {
      out.push_back(NodeToWstr(elem));
    } else {
      NodeThrowTypeError(name + L" must be a string or an array of strings");
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
    NodeThrowTypeError(name + L" must be a function");
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
    NodeThrowTypeError(name + L" must be an object");
    return false;
  }

  out = Nan::To<v8::Object>(info[index]).ToLocalChecked();
  return true;
}

bool NodeParam(Nan::NAN_METHOD_ARGS_TYPE info, int index,
               const std::wstring &name, anitomy::ElementCategory &out) {
  std::wstring categoryName;

  if (!NodeParam(info, index, name, categoryName)) {
    return false;
  }

  auto category = GetElementCategory(categoryName);

  if (category == anitomy::kElementUnknown) {
    NodeThrowError(name + L" must be a valid ElementCategory name");
    return false;
  }

  out = category;

  return true;
}

bool NodeObjectGet(v8::Local<v8::Object> obj, const std::wstring &objName,
                   const std::wstring &key, std::wstring &out) {
  auto val = Nan::Get(obj, WstrToNode(key));

  if (val.IsEmpty() || !val.ToLocalChecked()->IsString()) {
    NodeThrowTypeError(objName + L"." + key + L" must be a string");
    return false;
  }

  out = NodeToWstr(val.ToLocalChecked());
  return true;
}

bool NodeObjectGet(v8::Local<v8::Object> obj, const std::wstring &objName,
                   const std::wstring &key, std::vector<std::wstring> &out) {
  auto val = Nan::Get(obj, WstrToNode(key));

  if (val.IsEmpty() || !val.ToLocalChecked()->IsArray()) {
    NodeThrowTypeError(objName + L"." + key + L" must be an array of strings");
    return false;
  }

  auto arr = val.ToLocalChecked().As<v8::Array>();
  Nan::MaybeLocal<v8::Value> elem;

  for (auto i = 0U; i < arr->Length(); ++i) {
    elem = Nan::Get(arr, i);

    if (elem.IsEmpty() || !elem.ToLocalChecked()->IsString()) {
      NodeThrowTypeError(objName + L"." + key + L" must be an array of strings");
      return false;
    }

    out.push_back(NodeToWstr(elem.ToLocalChecked()));
  }

  return true;
}

bool NodeObjectGet(v8::Local<v8::Object> obj, const std::wstring &objName,
                   const std::wstring &key, bool &out) {
  auto val = Nan::Get(obj, WstrToNode(key));

  if (val.IsEmpty() || !val.ToLocalChecked()->IsBoolean()) {
    NodeThrowTypeError(objName + L"." + key + L" must be a boolean");
    return false;
  }

  out = Nan::To<bool>(val.ToLocalChecked()).FromMaybe(out);
  return true;
}
