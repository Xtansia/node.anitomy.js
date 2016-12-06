# node.anitomy.js
[![License](https://img.shields.io/badge/license-MPL--2.0-blue.svg?style=flat-square&maxAge=2592000)](https://github.com/Xtansia/node.anitomy.js/raw/master/LICENSE)
[![Linux & OSX Build Status](https://img.shields.io/travis/Xtansia/node.anitomy.js.svg?label=linux+%26+osx&style=flat-square&maxAge=2592000)](https://travis-ci.org/Xtansia/node.anitomy.js)
[![Windows Build Status](https://img.shields.io/appveyor/ci/Xtansia/node-anitomy-js.svg?label=windows&style=flat-square&maxAge=2592000)](https://ci.appveyor.com/project/Xtansia/node-anitomy-js)  
*node.anitomy.js* is a Node.js binding for [Anitomy](https://github.com/erengy/anitomy) a C++ library for parsing anime video filenames.

## Installing
You can install *node.anitomy.js* as a dependency of your project using NPM like so, provided you have met the dependencies listed below.
```
> npm install --save Xtansia/node.anitomy.js
```

### Dependencies
* Node.JS >= 0.10.0
* CMake >= 3.2
* A C++11 compatible compiler
  - GCC >= 4.9
  - Clang >= 3.3 (Probably?)
  - [Visual Studio 2015](https://www.visualstudio.com/products/vs-2015-product-editions) OR [Visual C++ Build Tools](http://landinghub.visualstudio.com/visual-cpp-build-tools)

### Note About Installing/Building
Before building the binding the buildscript will test if the compiler supports the `<codecvt>` header, 
if it doesn't, like GCC-4.9, the buildscript will automatically enable using Boost::Locale, 
downloading and building Boost using [boost-lib](https://github.com/cmake-js/boost-lib).
You can force using Boost::Locale by using `npm config` before installing or compiling like so:
```
> npm config set cmake_NODE_ANITOMY_USE_BOOST ON
```

## Example Usage
```javascript
const anitomy = require('anitomy');

const filename = '[TaigaSubs]_Toradora!_(2008)_-_01v2_-_Tiger_and_Dragon_[1280x720_H.264_FLAC][1234ABCD].mkv';

const elems = anitomy.parseSync(filename);

console.log(elems.AnimeTitle + ' #' + elems.EpisodeNumber + ' by ' + elems.ReleaseGroup);
```

## API
### parse(filenames[, options], callback)
Parses the filenames asynchronously, using the given [options](#parse-options) if provided,
calling the callback with the resulting AnitomyElements as the first parameter.
`filenames` can be either a string, or an array of strings.
If it is a string, then the callback will be passed an instance of AnitomyElements,
if it is an array of strings, then the callback will be passed an array of AnitomyElements.
```javascript
anitomy.parse('[TaigaSubs]_Toradora!_(2008)_-_01v2_-_Tiger_and_Dragon_[1280x720_H.264_FLAC][1234ABCD].mkv', function (elems) {
  console.log(elems.AnimeTitle + ' #' + elems.EpisodeNumber + ' by ' + elems.ReleaseGroup);
});
```

### parseSync(filenames[, options])
Parses the filenames synchronously, using the given [options](#parse-options) if provided, returning the resulting AnitomyElements.
`filenames` can be either a string, or an array of strings.
If it is a string, an instance of AnitomyElements will be returned,
if it is an array of strings, then an array of AnitomyElements will be returned.
```javascript
const elems = anitomy.parseSync('[TaigaSubs]_Toradora!_(2008)_-_01v2_-_Tiger_and_Dragon_[1280x720_H.264_FLAC][1234ABCD].mkv');
console.log(elems.AnimeTitle + ' #' + elems.EpisodeNumber + ' by ' + elems.ReleaseGroup);
```

### Parse Options
Options are given in the form of an object, with any or all of the following properties set, if unset they default to these values (Anitomy's defaults):
```javascript
{
  allowedDelimiters: ' _.&+,|',
  ignoredStrings: [],
  parseEpisodeNumber: true,
  parseEpisodeTitle: true,
  parseFileExtension: true,
  parseReleaseGroup: true
}
```

### AnitomyElements
This object represents the parsed elements of the filename. With a property for each ElementCategory, the properties are immutable. In cases where there are multiple elements in a category the property's value will be an array.
```javascript
AnitomyElements {
  AnimeTitle: 'Toradora!',
  AnimeYear: '2008',
  AudioTerm: 'FLAC',
  EpisodeNumber: '01',
  EpisodeTitle: 'Tiger and Dragon',
  FileChecksum: '1234ABCD',
  FileExtension: 'mkv',
  FileName: '[TaigaSubs]_Toradora!_(2008)_-_01v2_-_Tiger_and_Dragon_[1280x720_H.264_FLAC][1234ABCD]',
  ReleaseGroup: 'TaigaSubs',
  ReleaseVersion: '2',
  VideoResolution: '1280x720',
  VideoTerm: 'H.264' }
```

#### AnitomyElements#empty()
Returns true if the AnitomyElements has no category-value pairs. such as:
```javascript
var elems = anitomy.parseSync('')
// -> AnitomyElements {}
elems.empty()
// -> true
```

## To Be Implemented
  * Methods on AnitomyElements much like those on anitomy::Elements

## License
*node.anitomy.js* is licensed under [Mozilla Public License 2.0](https://www.mozilla.org/en-US/MPL/2.0/FAQ/).
