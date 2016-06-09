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
		tpl->SetClassName(LOCAL_STRING("ElementPair"));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		// Prototype
		Nan::SetAccessor(tpl->InstanceTemplate(), LOCAL_STRING("category"), GetCategory, READ_ONLY_SETTER);
		Nan::SetAccessor(tpl->InstanceTemplate(), LOCAL_STRING("value"), GetValue, READ_ONLY_SETTER);

		constructor().Reset(tpl->GetFunction());
		Nan::Set(target, LOCAL_STRING("ElementPair"), tpl->GetFunction());
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

	NAN_GETTER(ElementPair::GetCategory) {
		UNWRAP_OBJ(ElementPair);
		info.GetReturnValue().Set(LOCAL_WSTRING(obj->category_));
	}

	NAN_GETTER(ElementPair::GetValue) {
		UNWRAP_OBJ(ElementPair);
		info.GetReturnValue().Set(LOCAL_WSTRING(obj->value_));
	}
}