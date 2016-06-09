
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

#include <algorithm>
#include <map>
#include <nan.h>
#include <anitomy/element.h>

namespace objects {
	const std::map<std::wstring, anitomy::ElementCategory> ElementCategoryNames {
		{ L"AnimeSeason",			anitomy::kElementAnimeSeason },
		{ L"AnimeSeasonPrefix",		anitomy::kElementAnimeSeasonPrefix },
		{ L"AnimeTitle",			anitomy::kElementAnimeTitle },
		{ L"AnimeType",				anitomy::kElementAnimeType },
		{ L"AnimeYear",				anitomy::kElementAnimeYear },
		{ L"AudioTerm",				anitomy::kElementAudioTerm },
		{ L"DeviceCompatibility",	anitomy::kElementDeviceCompatibility },
		{ L"EpisodeNumber",			anitomy::kElementEpisodeNumber },
		{ L"EpisodeNumberAlt",		anitomy::kElementEpisodeNumberAlt },
		{ L"EpisodePrefix",			anitomy::kElementEpisodePrefix },
		{ L"EpisodeTitle",			anitomy::kElementEpisodeTitle },
		{ L"FileChecksum",			anitomy::kElementFileChecksum },
		{ L"FileExtension",			anitomy::kElementFileExtension },
		{ L"FileName",				anitomy::kElementFileName },
		{ L"Language",				anitomy::kElementLanguage },
		{ L"Other",					anitomy::kElementOther },
		{ L"ReleaseGroup",			anitomy::kElementReleaseGroup },
		{ L"ReleaseInformation",	anitomy::kElementReleaseInformation },
		{ L"ReleaseVersion",		anitomy::kElementReleaseVersion },
		{ L"Source",				anitomy::kElementSource },
		{ L"Subtitles",				anitomy::kElementSubtitles },
		{ L"VideoResolution",		anitomy::kElementVideoResolution },
		{ L"VideoTerm",				anitomy::kElementVideoTerm },
		{ L"VolumeNumber",			anitomy::kElementVolumeNumber },
		{ L"VolumePrefix",			anitomy::kElementVolumePrefix },
		{ L"Unknown",				anitomy::kElementUnknown }
	};

	inline const std::wstring GetNameForCategory(anitomy::ElementCategory category) {
		auto pair = std::find_if(ElementCategoryNames.begin(), ElementCategoryNames.end(), 
			[category] (const auto& it) { 
				return it.second == category;
			}
		);

		if (pair == ElementCategoryNames.end()) {
			return L"Unknown";
		}

		return pair->first;
	}

	class Elements : public Nan::ObjectWrap {
	public:
		static NAN_MODULE_INIT(Init);

	private:
		Elements(anitomy::Elements& elements) : elements_(elements) {};
		~Elements() {};

		static NAN_METHOD(New);
		static v8::Local<v8::Value> New(anitomy::Elements& elements);
		static NAN_METHOD(IsEmpty);
		static NAN_METHOD(Size);
		static NAN_METHOD(At);
		static NAN_INDEX_GETTER(IndexGetter);
		static NAN_INDEX_ENUMERATOR(IndexEnumerator);
		static NAN_METHOD(Get);
		static NAN_METHOD(GetAll);
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

		anitomy::Elements& elements_;

		friend class Anitomy;
	};

#define ELEMENT_CATEGORY_ARG(i) \
	if (info.Length() < i + 1) { \
		Nan::ThrowError("category must be given"); \
		return; \
	} \
	if (!info[i]->IsString()) { \
		Nan::ThrowTypeError("category must be a string"); \
		return; \
	} \
	auto elementCategoryNamesIt = ElementCategoryNames.find(StrToWstr(*Nan::Utf8String(info[i]))); \
	if (elementCategoryNamesIt == ElementCategoryNames.end()) { \
		Nan::ThrowTypeError("category is not a valid ElementCategory"); \
		return; \
	} \
	anitomy::ElementCategory category = elementCategoryNamesIt->second;

}

#endif // !ANITOMYJS_OBJECTS_ELEMENTS_H
