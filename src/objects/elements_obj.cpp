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

namespace objects {
	NAN_MODULE_INIT(Elements::Init) {
		Nan::HandleScope scope;

		// Prepare constructor template
		v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
		tpl->SetClassName(LOCAL_STRING("Elements"));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		// Prototype
		Nan::SetPrototypeMethod(tpl, "empty", IsEmpty);
		Nan::SetPrototypeMethod(tpl, "size", Size);
		Nan::SetPrototypeMethod(tpl, "at", At);
		Nan::SetIndexedPropertyHandler(tpl->InstanceTemplate(), IndexGetter, 0, 0, 0, IndexEnumerator);
		Nan::SetPrototypeMethod(tpl, "get", Get);
		Nan::SetPrototypeMethod(tpl, "get_all", GetAll);
		Nan::SetPrototypeMethod(tpl, "clear", Clear);
		Nan::SetPrototypeMethod(tpl, "insert", Insert);
		Nan::SetPrototypeMethod(tpl, "erase", Erase);
		Nan::SetPrototypeMethod(tpl, "count", Count);

		constructor().Reset(tpl->GetFunction());
		Nan::Set(target, LOCAL_STRING("Elements"), tpl->GetFunction());
	}

	NAN_METHOD(Elements::New) {
		if (!info.IsConstructCall()) {
			Nan::ThrowError("Cannot call constructor as function, you need to use 'new' keyword");
			return;
		}

		Elements* elements;

		if (info[0]->IsExternal()) {
			v8::Local<v8::External> ext = info[0].As<v8::External>();
			void* ptr = ext->Value();
			elements = static_cast<Elements*>(ptr);
		}
		else {
			if (info.Length() != 0) {
				Nan::ThrowError("Elements constructor doesn't take any arguments");
				return;
			}

			elements = new Elements(new anitomy::Elements(), true);
		}

		elements->Wrap(info.This());
		info.GetReturnValue().Set(info.This());
		return;
	}

	v8::Local<v8::Value> Elements::New(anitomy::Elements& elements) {
		Nan::EscapableHandleScope scope;

		Elements *wrapped = new Elements(&elements);

		v8::Local<v8::Value> ext = Nan::New<v8::External>(wrapped);
		v8::Local<v8::Object> obj = Nan::New(constructor())->NewInstance(1, &ext);

		return scope.Escape(obj);
	}

	NAN_METHOD(Elements::IsEmpty) {
		UNWRAP_OBJ(Elements);

		if (info.Length() < 1) { // empty()
			info.GetReturnValue().Set(obj->elements_->empty());
		}
		else { // empty(category)
			ELEMENT_CATEGORY_ARG(0, category);

			info.GetReturnValue().Set(obj->elements_->empty(category));
		}
	}

	NAN_METHOD(Elements::Size) {
		UNWRAP_OBJ(Elements);

		info.GetReturnValue().Set(static_cast<uint32_t>(obj->elements_->size()));
	}

	NAN_METHOD(Elements::At) {
		if (info.Length() < 1) {
			Nan::ThrowError("index must be given");
			return;
		}
		if (!info[0]->IsInt32() || !info[0]->IsUint32()) {
			Nan::ThrowTypeError("index must be an integer");
			return;
		}

		UNWRAP_OBJ(Elements);

		size_t index = static_cast<size_t>(info[0]->Uint32Value());

		if (index >= obj->elements_->size()) {
			Nan::ThrowRangeError("index out of range");
			return;
		}

		info.GetReturnValue().Set(ElementPair::New(obj->elements_->at(index)));
	}

	NAN_INDEX_GETTER(Elements::IndexGetter) {
		UNWRAP_OBJ(Elements);

		if (index >= obj->elements_->size()) {
			Nan::ThrowRangeError("index out of range");
			return;
		}

		info.GetReturnValue().Set(ElementPair::New((*obj->elements_)[static_cast<size_t>(index)]));
	}

	NAN_INDEX_ENUMERATOR(Elements::IndexEnumerator) {
		UNWRAP_OBJ(Elements);
		uint32_t size = static_cast<uint32_t>(obj->elements_->size());

		v8::Local<v8::Array> arr = Nan::New<v8::Array>(size);
		
		for (uint32_t i = 0; i < size; ++i) {
			arr->Set(i, Nan::New(i));
		}

		info.GetReturnValue().Set(arr);
	}

	NAN_METHOD(Elements::Get) {
		ELEMENT_CATEGORY_ARG(0, category);
		UNWRAP_OBJ(Elements);

		info.GetReturnValue().Set(LOCAL_WSTRING(obj->elements_->get(category)));
	}

	NAN_METHOD(Elements::GetAll) {
		ELEMENT_CATEGORY_ARG(0, category);
		UNWRAP_OBJ(Elements);

		std::vector<std::wstring> values = obj->elements_->get_all(category);
		uint32_t size = static_cast<uint32_t>(values.size());

		v8::Local<v8::Array> arr = Nan::New<v8::Array>(size);

		for (uint32_t i = 0; i < size; ++i) {
			arr->Set(i, LOCAL_WSTRING(values[i]));
		}

		info.GetReturnValue().Set(arr);
	}

	NAN_METHOD(Elements::Clear) {
		UNWRAP_OBJ(Elements);

		obj->elements_->clear();
	}

	NAN_METHOD(Elements::Insert) {
		ELEMENT_CATEGORY_ARG(0, category);
		STRING_ARG(1, value);
		UNWRAP_OBJ(Elements);

		obj->elements_->insert(category, value);
	}

	NAN_METHOD(Elements::Erase) {
		ELEMENT_CATEGORY_ARG(0, category);
		UNWRAP_OBJ(Elements);

		obj->elements_->erase(category);
	}

	NAN_METHOD(Elements::Count) {
		ELEMENT_CATEGORY_ARG(0, category);
		UNWRAP_OBJ(Elements);

		info.GetReturnValue().Set(static_cast<uint32_t>(obj->elements_->count(category)));
	}
}