/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once
#ifndef ANITOMYJS_OBJECTS_ELEMENTS_H
#define ANITOMYJS_OBJECTS_ELEMENTS_H

#include <nan.h>

namespace objects {
	class Elements : public Nan::ObjectWrap {
	public:
		static NAN_MODULE_INIT(Init);

	private:
		Elements() {};
		~Elements() {};

		static NAN_METHOD(New);
		static v8::Local<v8::Value> New(v8::Local<v8::Value> anitomy);
		static NAN_METHOD(IsEmpty);
		static NAN_METHOD(Size);
		static NAN_METHOD(At);
		//static NAN_METHOD(get);
		//static NAN_METHOD(getAll);
		//static NAN_METHOD(clear);
		//static NAN_METHOD(insert);
		//static NAN_METHOD(erase);
		//static NAN_METHOD(count);
		//static NAN_METHOD(find);
		//static NAN_METHOD(forEach);

		static inline Nan::Persistent<v8::Function>& constructor() {
			static Nan::Persistent<v8::Function> constructor_;
			return constructor_;
		}

		friend class Anitomy;
	};
}

#endif // !ANITOMYJS_OBJECTS_ELEMENTS_H
