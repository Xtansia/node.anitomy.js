/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "elements_obj.h"
#include "anitomy_obj.h"
#include "elementpair_obj.h"
#include "../util.h"

#include <anitomy/element.h>

namespace objects {
	NAN_MODULE_INIT(Elements::Init) {
		Nan::HandleScope scope;

		// Prepare constructor template
		v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
		tpl->SetClassName(Nan::New("Elements").ToLocalChecked());
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		// Prototype
		Nan::SetPrototypeMethod(tpl, "empty", IsEmpty);
		Nan::SetPrototypeMethod(tpl, "size", Size);
		Nan::SetPrototypeMethod(tpl, "at", At);
		Nan::SetIndexedPropertyHandler(tpl->InstanceTemplate(), IndexGetter, 0, 0, 0, IndexEnumerator);
		//Nan::SetPrototypeMethod(tpl, "get", get);
		//Nan::SetPrototypeMethod(tpl, "get_all", getAll);
		//Nan::SetPrototypeMethod(tpl, "clear", clear);
		//Nan::SetPrototypeMethod(tpl, "insert", insert);
		//Nan::SetPrototypeMethod(tpl, "erase", erase);
		//Nan::SetPrototypeMethod(tpl, "count", count);
		//Nan::SetPrototypeMethod(tpl, "find", find);
		//Nan::SetPrototypeMethod(tpl, "forEach", forEach);

		constructor().Reset(tpl->GetFunction());
		Nan::Set(target, Nan::New("Elements").ToLocalChecked(), tpl->GetFunction());
	}

	NAN_METHOD(Elements::New) {
		if (!info.IsConstructCall()) {
			Nan::ThrowError("Cannot call constructor as function, you need to use 'new' keyword");
			return;
		}

		if (info[0]->IsExternal()) {
			v8::Local<v8::External> ext = info[0].As<v8::External>();
			void* ptr = ext->Value();
			Elements *elements = static_cast<Elements *>(ptr);
			elements->Wrap(info.This());
			info.GetReturnValue().Set(info.This());
			return;
		}
		else {
			Nan::ThrowError("Cannot create Elements directly");
		}
	}

	v8::Local<v8::Value> Elements::New(v8::Local<v8::Value> anitomy) {
		Nan::EscapableHandleScope scope;

		Elements *wrapped = new Elements();

		v8::Local<v8::Value> ext = Nan::New<v8::External>(wrapped);
		v8::Local<v8::Object> obj = Nan::New(constructor())->NewInstance(1, &ext);
		obj->SetHiddenValue(Nan::New("anitomy_").ToLocalChecked(), anitomy);

		return scope.Escape(obj);
	}

	NAN_METHOD(Elements::IsEmpty) {
		v8::Local<v8::Object> obj = info.This()->GetHiddenValue(Nan::New("anitomy_").ToLocalChecked()).As<v8::Object>();
		Anitomy* anitomy = ObjectWrap::Unwrap<Anitomy>(obj);

		info.GetReturnValue().Set(anitomy->GetElements().empty());
	}

	NAN_METHOD(Elements::Size) {
		v8::Local<v8::Object> obj = info.This()->GetHiddenValue(Nan::New("anitomy_").ToLocalChecked()).As<v8::Object>();
		Anitomy* anitomy = ObjectWrap::Unwrap<Anitomy>(obj);

		info.GetReturnValue().Set(static_cast<uint32_t>(anitomy->GetElements().size()));
	}

	NAN_METHOD(Elements::At) {
		v8::Local<v8::Object> obj = info.This()->GetHiddenValue(Nan::New("anitomy_").ToLocalChecked()).As<v8::Object>();
		Anitomy* anitomy = ObjectWrap::Unwrap<Anitomy>(obj);

		if (info.Length() < 1) {
			Nan::ThrowError("index must be given");
			return;
		}
		if (!info[0]->IsInt32() || !info[0]->IsUint32()) {
			Nan::ThrowTypeError("index must be an integer");
			return;
		}

		size_t index = static_cast<size_t>(info[0]->Uint32Value());

		if (index >= anitomy->GetElements().size()) {
			Nan::ThrowRangeError("index out of range");
			return;
		}

		info.GetReturnValue().Set(ElementPair::New(anitomy->GetElements().at(index)));
	}

	NAN_INDEX_GETTER(Elements::IndexGetter) {
		v8::Local<v8::Object> obj = info.This()->GetHiddenValue(Nan::New("anitomy_").ToLocalChecked()).As<v8::Object>();
		Anitomy* anitomy = ObjectWrap::Unwrap<Anitomy>(obj);

		if (index >= anitomy->GetElements().size()) {
			Nan::ThrowRangeError("index out of range");
			return;
		}

		info.GetReturnValue().Set(ElementPair::New(anitomy->GetElements()[static_cast<size_t>(index)]));
	}

	NAN_INDEX_ENUMERATOR(Elements::IndexEnumerator) {
		v8::Local<v8::Object> obj = info.This()->GetHiddenValue(Nan::New("anitomy_").ToLocalChecked()).As<v8::Object>();
		Anitomy* anitomy = ObjectWrap::Unwrap<Anitomy>(obj);
		uint32_t size = static_cast<uint32_t>(anitomy->GetElements().size());

		v8::Local<v8::Array> arr = Nan::New<v8::Array>(size);
		
		for (uint32_t i = 0; i < size; ++i) {
			arr->Set(i, Nan::New(i));
		}

		info.GetReturnValue().Set(arr);
	}
}