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
		Nan::SetPrototypeMethod(tpl, "Parse", Parse);
		Nan::SetPrototypeMethod(tpl, "elements", GetElements);
		Nan::SetPrototypeMethod(tpl, "options", GetOptions);
		Nan::SetPrototypeMethod(tpl, "tokens", GetTokens);

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

		Nan::Utf8String filenameArg(info[0]);
		std::basic_string<wchar_t> filename(L"");
		if (filenameArg.length() > 0 && *filenameArg != NULL) {
			filename = StrToWstr(*filenameArg);
		}

		bool res = obj->anitomy_.Parse(filename);
		info.GetReturnValue().Set(res);
	}

	NAN_METHOD(Anitomy::GetElements) {
		info.GetReturnValue().Set(info.This()->GetHiddenValue(Nan::New("elements_").ToLocalChecked()));
	}

	NAN_METHOD(Anitomy::GetOptions) {
		// TODO: Get options
		Anitomy* obj = ObjectWrap::Unwrap<Anitomy>(info.Holder());
		info.GetReturnValue().Set(Nan::New("options").ToLocalChecked());
	}

	NAN_METHOD(Anitomy::GetTokens) {
		// TODO: Get tokens
		Anitomy* obj = ObjectWrap::Unwrap<Anitomy>(info.Holder());
		info.GetReturnValue().Set(Nan::New("tokens").ToLocalChecked());
	}
}