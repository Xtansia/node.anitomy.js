/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "anitomyjs.h"
#include "util.h"

NAN_MODULE_INIT(AnitomyObject::Init) {
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

NAN_METHOD(AnitomyObject::New) {
	if (info.IsConstructCall()) {
		// Invoked as constructor: `new AnitomyObject(...)`
		AnitomyObject* obj = new AnitomyObject();
		obj->Wrap(info.This());
		info.GetReturnValue().Set(info.This());
	}
	else {
		// Invoked as plain function: `AnitomyObject(...)`, turn into construct call.
		v8::Local<v8::Function> cons = Nan::New(constructor());
		info.GetReturnValue().Set(cons->NewInstance());
	}
}

NAN_METHOD(AnitomyObject::Parse) {
	AnitomyObject* obj = ObjectWrap::Unwrap<AnitomyObject>(info.Holder());

	if (info.Length() < 1 || !info[0]->IsString()) {
		Nan::ThrowTypeError("Expected 1 string argument");
		return;
	}

	Nan::Utf8String filenameArg(info[0]);
	std::wstring filename(L"");
	if (filenameArg.length() > 0 && *filenameArg != NULL) {
		filename = StrToWstr(*filenameArg);
	}

	bool res = obj->_anitomy.Parse(filename);
	info.GetReturnValue().Set(res);
}

NAN_METHOD(AnitomyObject::GetElements) {
	// TODO: Get elements
	AnitomyObject* obj = ObjectWrap::Unwrap<AnitomyObject>(info.Holder());
	info.GetReturnValue().Set(Nan::New("elements").ToLocalChecked());
}

NAN_METHOD(AnitomyObject::GetOptions) {
	// TODO: Get options
	AnitomyObject* obj = ObjectWrap::Unwrap<AnitomyObject>(info.Holder());
	info.GetReturnValue().Set(Nan::New("options").ToLocalChecked());
}

NAN_METHOD(AnitomyObject::GetTokens) {
	// TODO: Get tokens
	AnitomyObject* obj = ObjectWrap::Unwrap<AnitomyObject>(info.Holder());
	info.GetReturnValue().Set(Nan::New("tokens").ToLocalChecked());
}

NAN_MODULE_INIT(Init) {
	AnitomyObject::Init(target);
}

NODE_MODULE(anitomy, Init)