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

std::wstring StrToWstr(const std::string& input);

std::string WstrToStr(const std::wstring& input);

#endif // !ANITOMYJS_UTIL_H