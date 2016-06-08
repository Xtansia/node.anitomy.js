/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once
#ifndef ANITOMYJS_UTIL_H
#define ANITOMYJS_UTIL_H

#include <string>
#include <nan.h>

std::wstring StrToWstr(const std::string& input);
std::string WstrToStr(const std::wstring& input);

NAN_SETTER(READ_ONLY_SETTER);

#endif // !ANITOMYJS_UTIL_H