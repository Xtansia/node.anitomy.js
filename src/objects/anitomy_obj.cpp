/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "anitomy_obj.h"
#include "elements_obj.h"
#include "../util.h"

namespace objects {
	NAN_MODULE_INIT(Anitomy::Init) {
		Nan::HandleScope scope;

		// Prepare constructor template
		v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
		tpl->SetClassName(Nan::New("Anitomy").ToLocalChecked());
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		// Prototype
		Nan::SetPrototypeMethod(tpl, "parse", Parse);
		Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("elements").ToLocalChecked(), GetElements, READ_ONLY_SETTER);
		Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("options").ToLocalChecked(), GetOptions, READ_ONLY_SETTER);
		Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("tokens").ToLocalChecked(), GetTokens, READ_ONLY_SETTER);

		constructor().Reset(tpl->GetFunction());
		Nan::Set(target, Nan::New("Anitomy").ToLocalChecked(), tpl->GetFunction());
	}

	NAN_METHOD(Anitomy::New) {
		if (!info.IsConstructCall()) {
			Nan::ThrowError("Cannot call constructor as function, you need to use 'new' keyword");
			return;
		}
		if (info.Length() != 0) {
			Nan::ThrowError("Anitomy constructor doesn't take any arguments");
			return;
		}

		Anitomy* wrapped = new Anitomy();
		v8::Local<v8::Value> elements = Elements::New(wrapped->anitomy_.elements());
		info.This()->SetHiddenValue(Nan::New("elements_").ToLocalChecked(), elements);
		wrapped->Wrap(info.This());
		info.GetReturnValue().Set(info.This());
	}

	NAN_METHOD(Anitomy::Parse) {
		Anitomy* obj = ObjectWrap::Unwrap<Anitomy>(info.Holder());

		if (info.Length() < 1) {
			Nan::ThrowError("filename must be given");
			return;
		}
		if (!info[0]->IsString()) {
			Nan::ThrowTypeError("filename must be a string");
			return;
		}

		info.GetReturnValue().Set(obj->anitomy_.Parse(StrToWstr(*Nan::Utf8String(info[0]))));
	}

	NAN_GETTER(Anitomy::GetElements) {
		info.GetReturnValue().Set(info.This()->GetHiddenValue(Nan::New("elements_").ToLocalChecked()));
	}

	NAN_GETTER(Anitomy::GetOptions) {
		// TODO: Get options
		info.GetReturnValue().Set(Nan::New("options").ToLocalChecked());
	}

	NAN_GETTER(Anitomy::GetTokens) {
		// TODO: Get tokens
		info.GetReturnValue().Set(Nan::New("tokens").ToLocalChecked());
	}
}