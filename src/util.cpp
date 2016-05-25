/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <locale>
#include <codecvt>
#include "util.h"

template<class Facet>
struct deletable_facet : Facet {
	template<class ...Args>
	deletable_facet(Args&& ...args) : Facet(std::forward<Args>(args)...) {}
	~deletable_facet() {}
};

static std::wstring_convert<deletable_facet<std::codecvt<wchar_t, char, std::mbstate_t>>, wchar_t> wstrConv;

std::wstring StrToWstr(const std::string& input) {
	return wstrConv.from_bytes(input);
}

std::string WstrToStr(const std::wstring& input) {
	return wstrConv.to_bytes(input);
}