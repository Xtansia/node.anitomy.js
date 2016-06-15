/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "options_object.h"

void OptionsObject::Init() {
  Nan::HandleScope scope;

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(NODE_LOCAL_STRING("Options"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_ACCESSOR("allowed_delimiters", AllowedDelimiters);
  NODE_SET_ACCESSOR("ignored_strings", IgnoredStrings);
  NODE_SET_ACCESSOR("parse_episode_number", ParseEpisodeNumber);
  NODE_SET_ACCESSOR("parse_file_extension", ParseFileExtension);
  NODE_SET_ACCESSOR("parse_release_group", ParseReleaseGroup);
    
  constructor().Reset(tpl->GetFunction());
}

NAN_METHOD(OptionsObject::New) {
  if (!info.IsConstructCall()) {
    Nan::ThrowError("Cannot call constructor as function, you need to use 'new' keyword");
    return;
  }

  if (info[0]->IsExternal()) {
    v8::Local<v8::External> ext = info[0].As<v8::External>();
    void* ptr = ext->Value();
    OptionsObject *options = static_cast<OptionsObject*>(ptr);
    options->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
    return;
  }
  else {
    Nan::ThrowError("Cannot create Options directly");
  }
}

v8::Local<v8::Value> OptionsObject::New(anitomy::Options& options) {
  Nan::EscapableHandleScope scope;

  OptionsObject* wrapped = new OptionsObject(options);

  v8::Local<v8::Value> ext = Nan::New<v8::External>(wrapped);
  v8::Local<v8::Object> obj = Nan::New(constructor())->NewInstance(1, &ext);

  return scope.Escape(obj);
}

NAN_GETTER(OptionsObject::GetAllowedDelimiters) {
  NODE_UNWRAP_OBJ(OptionsObject);

  info.GetReturnValue().Set(NODE_LOCAL_WSTRING(obj->options_.allowed_delimiters));
}

NAN_SETTER(OptionsObject::SetAllowedDelimiters) {
  NODE_UNWRAP_OBJ(OptionsObject);
  NODE_ENSURE_VALUE_STRING("allowed_delimiters", value);

  obj->options_.allowed_delimiters = StrToWstr(*Nan::Utf8String(value));
}

NAN_GETTER(OptionsObject::GetIgnoredStrings) {
  NODE_UNWRAP_OBJ(OptionsObject);

  uint32_t size = static_cast<uint32_t>(obj->options_.ignored_strings.size());

  v8::Local<v8::Array> arr = Nan::New<v8::Array>(size);

  for (uint32_t i = 0; i < size; ++i) {
    arr->Set(i, NODE_LOCAL_WSTRING(obj->options_.ignored_strings.at(i)));
  }

  info.GetReturnValue().Set(arr);
}

NAN_SETTER(OptionsObject::SetIgnoredStrings) {
  NODE_UNWRAP_OBJ(OptionsObject);

  if (!value->IsArray()) {
    Nan::ThrowTypeError("ignored_strings must be an array of strings");
    return;
  }

  v8::Local<v8::Array> arr = value.As<v8::Array>();
  uint32_t size = arr->Length();

  std::vector<std::wstring> ignored_strings(static_cast<size_t>(size));

  for (uint32_t i = 0; i < size; ++i) {
    if (!arr->Get(i)->IsString()) {
      Nan::ThrowTypeError("ignored_strings must be an array of strings");
      return;
    }

    ignored_strings[i] = StrToWstr(*Nan::Utf8String(arr->Get(i)));
  }

  obj->options_.ignored_strings.clear();
  obj->options_.ignored_strings.insert(obj->options_.ignored_strings.end(), ignored_strings.begin(), ignored_strings.end());
}

NAN_GETTER(OptionsObject::GetParseEpisodeNumber) {
  NODE_UNWRAP_OBJ(OptionsObject);

  info.GetReturnValue().Set(obj->options_.parse_episode_number);
}

NAN_SETTER(OptionsObject::SetParseEpisodeNumber) {
  NODE_UNWRAP_OBJ(OptionsObject);
  NODE_ENSURE_VALUE_BOOLEAN("parse_episode_number", value);

  obj->options_.parse_episode_number = Nan::To<bool>(value).FromJust();
}

NAN_GETTER(OptionsObject::GetParseEpisodeTitle) {
  NODE_UNWRAP_OBJ(OptionsObject);

  info.GetReturnValue().Set(obj->options_.parse_episode_title);
}

NAN_SETTER(OptionsObject::SetParseEpisodeTitle) {
  NODE_UNWRAP_OBJ(OptionsObject);
  NODE_ENSURE_VALUE_BOOLEAN("parse_episode_title", value);

  obj->options_.parse_episode_title = Nan::To<bool>(value).FromJust();
}

NAN_GETTER(OptionsObject::GetParseFileExtension) {
  NODE_UNWRAP_OBJ(OptionsObject);

  info.GetReturnValue().Set(obj->options_.parse_file_extension);
}

NAN_SETTER(OptionsObject::SetParseFileExtension) {
  NODE_UNWRAP_OBJ(OptionsObject);
  NODE_ENSURE_VALUE_BOOLEAN("parse_file_extension", value);

  obj->options_.parse_file_extension = Nan::To<bool>(value).FromJust();
}

NAN_GETTER(OptionsObject::GetParseReleaseGroup) {
  NODE_UNWRAP_OBJ(OptionsObject);

  info.GetReturnValue().Set(obj->options_.parse_release_group);
}

NAN_SETTER(OptionsObject::SetParseReleaseGroup) {
  NODE_UNWRAP_OBJ(OptionsObject);
  NODE_ENSURE_VALUE_BOOLEAN("parse_release_group", value);

  obj->options_.parse_release_group = Nan::To<bool>(value).FromJust();
}