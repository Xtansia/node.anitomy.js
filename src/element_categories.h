/*
** Copyright (c) 2016-2017, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#pragma once
#ifndef ANITOMY_LIB_ELEMENT_CATEGORIES_H
#define ANITOMY_LIB_ELEMENT_CATEGORIES_H

#include "utils.h"

#include <anitomy/element.h>
#include <map>

#define ELEMENT_CATEGORIES                                                     \
  ENUM(AnimeSeason)                                                            \
  ENUM(AnimeSeasonPrefix)                                                      \
  ENUM(AnimeTitle)                                                             \
  ENUM(AnimeType)                                                              \
  ENUM(AnimeYear)                                                              \
  ENUM(AudioTerm)                                                              \
  ENUM(DeviceCompatibility)                                                    \
  ENUM(EpisodeNumber)                                                          \
  ENUM(EpisodeNumberAlt)                                                       \
  ENUM(EpisodePrefix)                                                          \
  ENUM(EpisodeTitle)                                                           \
  ENUM(FileChecksum)                                                           \
  ENUM(FileExtension)                                                          \
  ENUM(FileName)                                                               \
  ENUM(Language)                                                               \
  ENUM(Other)                                                                  \
  ENUM(ReleaseGroup)                                                           \
  ENUM(ReleaseInformation)                                                     \
  ENUM(ReleaseVersion)                                                         \
  ENUM(Source)                                                                 \
  ENUM(Subtitles)                                                              \
  ENUM(VideoResolution)                                                        \
  ENUM(VideoTerm)                                                              \
  ENUM(VolumeNumber)                                                           \
  ENUM(VolumePrefix)

const std::vector<anitomy::ElementCategory> ElementCategories{
#define ENUM(name) anitomy::kElement##name,
    ELEMENT_CATEGORIES
#undef ENUM
};

inline anitomy::ElementCategory GetElementCategory(const std::wstring &name) {
  static const std::map<std::wstring, anitomy::ElementCategory> nameToCategory{
#define ENUM(name) {WIDE_STRINGIFY(name), anitomy::kElement##name},
      ELEMENT_CATEGORIES
#undef ENUM
  };

  auto it = nameToCategory.find(name);

  if (it == nameToCategory.end()) {
    return anitomy::kElementUnknown;
  }

  return it->second;
}

inline std::wstring GetName(anitomy::ElementCategory category) {
  static const std::map<anitomy::ElementCategory, std::wstring> categoryToName{
#define ENUM(name) {anitomy::kElement##name, WIDE_STRINGIFY(name)},
      ELEMENT_CATEGORIES
#undef ENUM
  };

  auto it = categoryToName.find(category);

  if (it == categoryToName.end()) {
    return L"Unknown";
  }

  return it->second;
}

#undef ELEMENT_CATEGORIES

#endif // !ANITOMY_LIB_ELEMENT_CATEGORIES_H
