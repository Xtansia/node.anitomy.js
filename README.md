# node.anitomy.js
[![License](https://img.shields.io/badge/license-MPL--2.0-blue.svg?style=flat-square&maxAge=2592000)](https://github.com/Xtansia/node.anitomy.js/raw/master/LICENSE)
[![Linux & OSX Build Status](https://img.shields.io/travis/Xtansia/node.anitomy.js.svg?label=linux+%26+osx&style=flat-square&maxAge=2592000)](https://travis-ci.org/Xtansia/node.anitomy.js)
[![Windows Build Status](https://img.shields.io/appveyor/ci/Xtansia/node-anitomy-js.svg?label=windows&style=flat-square&maxAge=2592000)](https://ci.appveyor.com/project/Xtansia/node-anitomy-js)  
*node.anitomy.js* is a Node.js binding for [Anitomy](https://github.com/erengy/anitomy) a C++ library for parsing anime video filenames.

## Installing
You can install *node.anitomy.js* as a dependency of your project using NPM like so, provided you have met the dependencies listed below.
```shell
> npm install --save Xtansia/node.anitomy.js
```

### Dependencies
* Node.JS >= 0.10.0
* CMake >= 3.2
* A C++11 compatible compiler
  - GCC >= 4.9
  - Clang >= 3.5 (Uncertain could be higher)
  - [Visual Studio 2015](https://www.visualstudio.com/products/vs-2015-product-editions) OR [Visual C++ Build Tools](http://landinghub.visualstudio.com/visual-cpp-build-tools)

## Example Usage
```javascript
const anitomy = require('anitomy');

const filename = '[TaigaSubs]_Toradora!_(2008)_-_01v2_-_Tiger_and_Dragon_[1280x720_H.264_FLAC][1234ABCD].mkv';

const elems = anitomy.parseSync(filename);

console.log(elems.AnimeTitle + ' #' + elems.EpisodeNumber + ' by ' + elems.ReleaseGroup);
```

## API
### parse(filename, callback)
Parses the filename asynchronously, calling the callback with an instance of AnitomyElements as the first parameter. 
```javascript
anitomy.parse('[TaigaSubs]_Toradora!_(2008)_-_01v2_-_Tiger_and_Dragon_[1280x720_H.264_FLAC][1234ABCD].mkv', function (elems) {
  console.log(elems.AnimeTitle + ' #' + elems.EpisodeNumber + ' by ' + elems.ReleaseGroup);
});
```

### parseSync(filename)
Parses the filename asynchronously, returning an instance of AnitomyElements.
```javascript
const elems = anitomy.parseSync('[TaigaSubs]_Toradora!_(2008)_-_01v2_-_Tiger_and_Dragon_[1280x720_H.264_FLAC][1234ABCD].mkv');
console.log(elems.AnimeTitle + ' #' + elems.EpisodeNumber + ' by ' + elems.ReleaseGroup);
```

### AnitomyElements
This object represents the parsed elements of the filename. With a property for each ElementCategory, the properties are immutable. In cases where there are multiple elements in a category the property's value will be an array.
```javascript
AnitomyElements {
  VolumePrefix: undefined,
  VolumeNumber: undefined,
  VideoTerm: 'H.264',
  VideoResolution: '1280x720',
  Unknown: undefined,
  Subtitles: undefined,
  Source: undefined,
  ReleaseVersion: '2',
  ReleaseInformation: undefined,
  ReleaseGroup: 'TaigaSubs',
  Other: undefined,
  Language: undefined,
  FileName: '[TaigaSubs]_Toradora!_(2008)_-_01v2_-_Tiger_and_Dragon_[1280x720_H.264_FLAC][1234ABCD]',
  FileExtension: 'mkv',
  FileChecksum: '1234ABCD',
  EpisodeTitle: undefined,
  EpisodePrefix: undefined,
  EpisodeNumberAlt: undefined,
  EpisodeNumber: '01',
  DeviceCompatibility: undefined,
  AudioTerm: 'FLAC',
  AnimeYear: '2008',
  AnimeType: undefined,
  AnimeTitle: 'Toradora!',
  AnimeSeasonPrefix: undefined,
  AnimeSeason: undefined }
```

## To Be Implemented
  * Options handling
  * Methods on AnitomyElements much like those on anitomy::Elements

## License
*node.anitomy.js* is licensed under [Mozilla Public License 2.0](https://www.mozilla.org/en-US/MPL/2.0/FAQ/).
