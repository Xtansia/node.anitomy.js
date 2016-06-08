/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "elementpair_obj.h"
#include "../util.h"

namespace objects {
	NAN_MODULE_INIT(ElementPair::Init) {
		Nan::HandleScope scope;

		// Prepare constructor template
		v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
		tpl->SetClassName(Nan::New("ElementPair").ToLocalChecked());
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		// Prototype
		Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("first").ToLocalChecked(), GetFirst, READ_ONLY_SETTER);
		Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("second").ToLocalChecked(), GetSecond, READ_ONLY_SETTER);

		constructor().Reset(tpl->GetFunction());
		Nan::Set(target, Nan::New("ElementPair").ToLocalChecked(), tpl->GetFunction());
	}

	NAN_METHOD(ElementPair::New) {
		if (!info.IsConstructCall()) {
			Nan::ThrowError("Cannot call constructor as function, you need to use 'new' keyword");
			return;
		}

		if (info[0]->IsExternal()) {
			v8::Local<v8::External> ext = info[0].As<v8::External>();
			void* ptr = ext->Value();
			ElementPair *elementPair = static_cast<ElementPair *>(ptr);
			elementPair->Wrap(info.This());
			info.GetReturnValue().Set(info.This());
			return;
		}
		else {
			Nan::ThrowError("Cannot create ElementPair directly");
		}
	}

	v8::Local<v8::Value> ElementPair::New(const anitomy::element_pair_t& pair) {
		Nan::EscapableHandleScope scope;

		ElementPair* wrapped = new ElementPair(pair);

		v8::Local<v8::Value> ext = Nan::New<v8::External>(wrapped);
		v8::Local<v8::Object> obj = Nan::New(constructor())->NewInstance(1, &ext);
		
		return scope.Escape(obj);
	}

	NAN_GETTER(ElementPair::GetFirst) {
		ElementPair* elementPair = ObjectWrap::Unwrap<ElementPair>(info.Holder());
		info.GetReturnValue().Set(elementPair->pair_.first);
	}

	NAN_GETTER(ElementPair::GetSecond) {
		ElementPair* elementPair = ObjectWrap::Unwrap<ElementPair>(info.Holder());
		info.GetReturnValue().Set(Nan::New(WstrToStr(elementPair->pair_.second)).ToLocalChecked());
	}
}