/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once
#ifndef ANITOMYJS_ANITOMYJS_H
#define ANITOMYJS_ANITOMYJS_H

#include <nan.h>
#include <anitomy/anitomy.h>

class AnitomyObject : public Nan::ObjectWrap {
public:
	static NAN_MODULE_INIT(Init);

private:
	AnitomyObject() {};
	~AnitomyObject() {};

	static NAN_METHOD(New);
	static NAN_METHOD(Parse);
	static NAN_METHOD(GetElements);
	static NAN_METHOD(GetOptions);
	static NAN_METHOD(GetTokens);

	static inline Nan::Persistent<v8::Function>& constructor() {
		static Nan::Persistent<v8::Function> my_constructor;
		return my_constructor;
	}

	anitomy::Anitomy _anitomy;
};

#endif // !ANITOMYJS_ANITOMYJS_H