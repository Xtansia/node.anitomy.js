/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once
#ifndef ANITOMYJS_OBJECTS_ANITOMY_H
#define ANITOMYJS_OBJECTS_ANITOMY_H

#include <nan.h>
#include <anitomy/anitomy.h>

namespace objects {
	class Elements;

	class Anitomy : public Nan::ObjectWrap {
	public:
		static NAN_MODULE_INIT(Init);

	private:
		Anitomy() {};
		~Anitomy() {};

		inline anitomy::Elements& GetElements() {
			return anitomy_.elements();
		}

		static NAN_METHOD(New);
		static NAN_METHOD(Parse);
		static NAN_METHOD(GetElements);
		static NAN_METHOD(GetOptions);
		static NAN_METHOD(GetTokens);

		static inline Nan::Persistent<v8::Function>& constructor() {
			static Nan::Persistent<v8::Function> constructor_;
			return constructor_;
		}

		anitomy::Anitomy anitomy_;

		friend class Elements;
	};
}

#endif // !ANITOMYJS_OBJECTS_ANITOMY_H