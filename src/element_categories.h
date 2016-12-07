/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once
#ifndef ANITOMY_LIB_ELEMENT_CATEGORIES_H
#define ANITOMY_LIB_ELEMENT_CATEGORIES_H

#include "utils.h"

#include <map>
#include <anitomy/element.h>

#define MAP_ELEMENT_CATEGORIES \
  MAP_ENUM(AnimeSeason) \
  MAP_ENUM(AnimeSeasonPrefix) \
  MAP_ENUM(AnimeTitle) \
  MAP_ENUM(AnimeType) \
  MAP_ENUM(AnimeYear) \
  MAP_ENUM(AudioTerm) \
  MAP_ENUM(DeviceCompatibility) \
  MAP_ENUM(EpisodeNumber) \
  MAP_ENUM(EpisodeNumberAlt) \
  MAP_ENUM(EpisodePrefix) \
  MAP_ENUM(EpisodeTitle) \
  MAP_ENUM(FileChecksum) \
  MAP_ENUM(FileExtension) \
  MAP_ENUM(FileName) \
  MAP_ENUM(Language) \
  MAP_ENUM(Other) \
  MAP_ENUM(ReleaseGroup) \
  MAP_ENUM(ReleaseInformation) \
  MAP_ENUM(ReleaseVersion) \
  MAP_ENUM(Source) \
  MAP_ENUM(Subtitles) \
  MAP_ENUM(VideoResolution) \
  MAP_ENUM(VideoTerm) \
  MAP_ENUM(VolumeNumber) \
  MAP_ENUM(VolumePrefix) \
  MAP_ENUM(Unknown)

const std::map<anitomy::ElementCategory, std::wstring> ElementCategoryToName {
#define MAP_ENUM(name) { anitomy::kElement##name, WIDE_STRINGIFY(name) },
  MAP_ELEMENT_CATEGORIES
#undef MAP_ENUM
};

const std::map<std::wstring, anitomy::ElementCategory> NameToElementCategory {
#define MAP_ENUM(name) { WIDE_STRINGIFY(name), anitomy::kElement##name },
  MAP_ELEMENT_CATEGORIES
#undef MAP_ENUM
};

#undef MAP_ELEMENT_CATEGORIES

#endif // !ANITOMY_LIB_ELEMENT_CATEGORIES_H
