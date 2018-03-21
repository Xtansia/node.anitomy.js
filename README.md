# node.anitomy.js
[![License](https://img.shields.io/badge/license-MPL--2.0-blue.svg?style=flat-square&maxAge=2592000)](https://github.com/Xtansia/node.anitomy.js/raw/master/LICENSE)
[![Linux & OSX Build Status](https://img.shields.io/travis/Xtansia/node.anitomy.js/master.svg?label=linux+%26+osx&style=flat-square&maxAge=2592000)](https://travis-ci.org/Xtansia/node.anitomy.js)
[![Windows Build Status](https://img.shields.io/appveyor/ci/Xtansia/node-anitomy-js/master.svg?label=windows&style=flat-square&maxAge=2592000)](https://ci.appveyor.com/project/Xtansia/node-anitomy-js)  
[![NPM](https://nodei.co/npm/anitomyjs.png)](https://nodei.co/npm/anitomyjs/)  
*node.anitomy.js* is a Node.js binding for
[Anitomy](https://github.com/erengy/anitomy) a C++ library for parsing anime
video filenames.

## Installing
You can install *node.anitomy.js* as a dependency of your project using NPM like
so, provided you have met the dependencies listed below.
```
> npm install --save anitomyjs
```

### Dependencies
* Node.JS >= 4.0.0
  - Can also be used with [NW.js](https://github.com/nwjs/nw.js) and 
    [Electron](https://github.com/atom/electron) by setting the desired runtime
    in your applications `package.json` as explained 
    [here](https://github.com/cmake-js/cmake-js#runtimes).
* CMake >= 3.2
* A C++11 compatible compiler
  - GCC >= 5
  - Clang >= 3.3 (Probably?)
  - [Visual Studio 2015](https://www.visualstudio.com/products/vs-2015-product-editions) 
    OR [Visual C++ Build Tools](http://landinghub.visualstudio.com/visual-cpp-build-tools)

## Example Usage
```javascript
const anitomy = require('anitomyjs');

const filename = '[TaigaSubs]_Toradora!_(2008)_-_01v2_-_Tiger_and_Dragon_[1280x720_H.264_FLAC][1234ABCD].mkv';

const elems = anitomy.parseSync(filename);

console.log(elems.AnimeTitle + ' #' + elems.EpisodeNumber + 
            ' by ' + elems.ReleaseGroup);
```
Would output:
```
Toradora! #01 by TaigaSubs
```

## API
### parse(filenames[, options], callback)
Parses the filenames asynchronously.
* `filenames` must be either a string, or an array of strings.
* `options` is optional, if provided it must be an object, as set out
  [here](#parse-options).
* `callback` must be a function, will be called passing the resulting elements
  as the first parameter.
  - If `filenames` was an array, then the passed value will be an array of
    [AnitomyElements](#anitomyelements), with the same ordering as `filenames`.
  - If `filenames` was a string, then the passed value will be an
    [AnitomyElements](#anitomyelements).
```javascript
const filename = '[TaigaSubs]_Toradora!_(2008)_-_01v2_-_Tiger_and_Dragon_[1280x720_H.264_FLAC][1234ABCD].mkv';
anitomy.parse(filename, function (elems) {
  console.log(elems.AnimeTitle + ' #' + elems.EpisodeNumber + 
              ' by ' + elems.ReleaseGroup);
});
```
Would output:
```
Toradora! #01 by TaigaSubs
```

### parseSync(filenames[, options])
Parses the filenames synchronously.
* `filenames` must be either a string, or an array of strings.
* `options` is optional, if provided it must be an object, as set out 
  [here](#parse-options).
* Returns:
  - If `filenames` was an array, then the returned value will be an array of
    [AnitomyElements](#anitomyelements), with the same ordering as `filenames`.
  - If `filenames` was a string, then the returned value will be an
    [AnitomyElements](#anitomyelements).
```javascript
const filename = '[TaigaSubs]_Toradora!_(2008)_-_01v2_-_Tiger_and_Dragon_[1280x720_H.264_FLAC][1234ABCD].mkv';
const elems = anitomy.parseSync(filename);
console.log(elems.AnimeTitle + ' #' + elems.EpisodeNumber + 
            ' by ' + elems.ReleaseGroup);
```
Would output:
```
Toradora! #01 by TaigaSubs
```

### parseEach(filenames[, options], callback)
Parses each of the individual filenames asynchronously, as opposed to 
[parse](#parsefilenames-options-callback) which parses them all in bulk. As it
is asynchronous there is no ordering ensured.
* `filenames` must be either a string, or an array of strings.
* `options` is optional, if provided it must be an object, as set out
  [here](#parse-options).
* `callback` must be a function, will be called when each filename is parsed,
  passing the filename and the resulting [AnitomyElements](#anitomyelements) as
  the first and second parameters respectively.
```javascript
const filenames = [
  '[TaigaSubs]_Toradora!_(2008)_-_01v2_-_Tiger_and_Dragon_[1280x720_H.264_FLAC][1234ABCD].mkv',
  '[ANBU]_Princess_Lover!_-_01_[2048A39A].mkv',
  '[ANBU-Menclave]_Canaan_-_01_[1024x576_H.264_AAC][12F00E89].mkv',
  '[ANBU-umai]_Haiyoru!_Nyaru-Ani_[596DD8E6].mkv'
];
anitomy.parseEach(filenames, function (filename, elems) {
  console.log(filename + '\n\t=> ' + elems.AnimeTitle + ' #' + elems.EpisodeNumber + 
              ' by ' + elems.ReleaseGroup);
});
```
Would output (though not necessarily in this order):
```
[ANBU]_Princess_Lover!_-_01_[2048A39A].mkv
  => Princess Lover! #01 by ANBU
[ANBU-Menclave]_Canaan_-_01_[1024x576_H.264_AAC][12F00E89].mkv
  => Canaan #01 by ANBU-Menclave
[ANBU-umai]_Haiyoru!_Nyaru-Ani_[596DD8E6].mkv
  => Haiyoru! Nyaru-Ani #undefined by ANBU-umai
[TaigaSubs]_Toradora!_(2008)_-_01v2_-_Tiger_and_Dragon_[1280x720_H.264_FLAC][1234ABCD].mkv
  => Toradora! #01 by TaigaSubs
```

### parseEachSync(filenames[, options], callback)
Same as [parseEach](#parseeachfilenames-options-callback) but synchronous, i.e.
calls callback for each parsed filename, then returns. `callback` is ensured to
be called in the same order as `filenames`.
* `filenames` must be either a string, or an array of strings.
* `options` is optional, if provided it must be an object, as set out
  [here](#parse-options).
* `callback` must be a function, will be called when each filename is parsed,
  passing the filename and the resulting [AnitomyElements](#anitomyelements) as
  the first and second parameters respectively.
```javascript
const filenames = [
  '[TaigaSubs]_Toradora!_(2008)_-_01v2_-_Tiger_and_Dragon_[1280x720_H.264_FLAC][1234ABCD].mkv',
  '[ANBU]_Princess_Lover!_-_01_[2048A39A].mkv',
  '[ANBU-Menclave]_Canaan_-_01_[1024x576_H.264_AAC][12F00E89].mkv',
  '[ANBU-umai]_Haiyoru!_Nyaru-Ani_[596DD8E6].mkv'
];
anitomy.parseEachSync(filenames, function (filename, elems) {
  console.log(filename + '\n\t=> ' + elems.AnimeTitle + ' #' + elems.EpisodeNumber + 
              ' by ' + elems.ReleaseGroup);
});
console.log('parseEachSync completed');
```
Would output:
```
[TaigaSubs]_Toradora!_(2008)_-_01v2_-_Tiger_and_Dragon_[1280x720_H.264_FLAC][1234ABCD].mkv
  => Toradora! #01 by TaigaSubs
[ANBU]_Princess_Lover!_-_01_[2048A39A].mkv
  => Princess Lover! #01 by ANBU
[ANBU-Menclave]_Canaan_-_01_[1024x576_H.264_AAC][12F00E89].mkv
  => Canaan #01 by ANBU-Menclave
[ANBU-umai]_Haiyoru!_Nyaru-Ani_[596DD8E6].mkv
  => Haiyoru! Nyaru-Ani #undefined by ANBU-umai
parseEachSync completed
```

### Parse Options
Options are given in the form of an object, with any or all of the following 
properties set, if unset they default to these values (Anitomy's defaults):
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
This object represents the parsed elements of the filename. With a property for
each [ElementCategory](#element-categories) that it has a value for, the 
properties are immutable. In cases where there are multiple elements in a 
category the property's value will be an array.
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

#### AnitomyElements#empty([category])
Returns true if the AnitomyElements has no category-value pairs.
* `category` is optional, if provided must be a string representing an
  [ElementCategory](#element-categories), return value will then be true if the 
  AnitomyElements has no values for that category.
```javascript
const filename = '[TaigaSubs]_Toradora!_(2008)_-_01v2_-_Tiger_and_Dragon_[1280x720_H.264_FLAC][1234ABCD].mkv';
const elems = anitomy.parseSync(filename);
/* -> AnitomyElements {
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
  VideoTerm: 'H.264' } */
const isEmpty = elems.empty();
// -> false
const hasNoVolumeNumber = elems.empty('VolumeNumber');
// -> true
```

#### AnitomyElements#count(category)
Returns the number of values associated with the category in this AnitomyElements.
* `category` must be a string representing an [ElementCategory](#element-categories).
```javascript
const filename = 'Detective Conan - 316-317 [DCTP][2411959B].mkv';
const elems = anitomy.parseSync(filename);
/* -> AnitomyElements {
  AnimeTitle: 'Detective Conan',
  EpisodeNumber: [ '316', '317' ],
  FileChecksum: '2411959B',
  FileExtension: 'mkv',
  FileName: 'Detective Conan - 316-317 [DCTP][2411959B]',
  ReleaseGroup: 'DCTP' } */
const numOfEpNumbers = elems.count('EpisodeNumber');
// -> 2
```

#### AnitomyElements#size()
Returns the number of category-value pairs this AnitomyElements has.
```javascript
const filename = '[TaigaSubs]_Toradora!_(2008)_-_01v2_-_Tiger_and_Dragon_[1280x720_H.264_FLAC][1234ABCD].mkv';
const elems = anitomy.parseSync(filename);
/* -> AnitomyElements {
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
  VideoTerm: 'H.264' } */
const size = elems.size();
// -> 12
```

#### AnitomyElements#get(category)
Returns the first value associated with the category in this AnitomyElements,
or an empty string if none.
* `category` must be a string representing an [ElementCategory](#element-categories).
```javascript
const filename = 'Detective Conan - 316-317 [DCTP][2411959B].mkv';
const elems = anitomy.parseSync(filename);
/* -> AnitomyElements {
  AnimeTitle: 'Detective Conan',
  EpisodeNumber: [ '316', '317' ],
  FileChecksum: '2411959B',
  FileExtension: 'mkv',
  FileName: 'Detective Conan - 316-317 [DCTP][2411959B]',
  ReleaseGroup: 'DCTP' } */
const firstEpNum = elems.get('EpisodeNumber');
// -> '316'
```

#### AnitomyElements#getAll(category)
Returns an array of all the values associated with the category in this 
AnitomyElements, or an empty array if none.
* `category` must be a string representing an [ElementCategory](#element-categories).
```javascript
const filename = 'Detective Conan - 316-317 [DCTP][2411959B].mkv';
const elems = anitomy.parseSync(filename);
/* -> AnitomyElements {
  AnimeTitle: 'Detective Conan',
  EpisodeNumber: [ '316', '317' ],
  FileChecksum: '2411959B',
  FileExtension: 'mkv',
  FileName: 'Detective Conan - 316-317 [DCTP][2411959B]',
  ReleaseGroup: 'DCTP' } */
const epNums = elems.getAll('EpisodeNumber');
// -> [ '316', '317' ]
```

### Element Categories
The element category names are the same as in `anitomy::ElementCategory`, 
without the `anitomy::kElement` prefix:
```javascript
[
  'AnimeSeason',
  'AnimeSeasonPrefix',
  'AnimeTitle',
  'AnimeType',
  'AnimeYear',
  'AudioTerm',
  'DeviceCompatibility',
  'EpisodeNumber',
  'EpisodeNumberAlt',
  'EpisodePrefix',
  'EpisodeTitle',
  'FileChecksum',
  'FileExtension',
  'FileName',
  'Language',
  'Other',
  'ReleaseGroup',
  'ReleaseInformation',
  'ReleaseVersion',
  'Source',
  'Subtitles',
  'VideoResolution',
  'VideoTerm',
  'VolumeNumber',
  'VolumePrefix'
]
```

## To Be Implemented
  * Methods on AnitomyElements much like those on anitomy::Elements

## License
*node.anitomy.js* is licensed under
[Mozilla Public License 2.0](https://www.mozilla.org/en-US/MPL/2.0/FAQ/).
