# node.anitomy.js
NodeJS binding for [Anitomy](https://github.com/erengy/anitomy), an anime video filename parsing library.

## Building it
This addon uses [cmake-js](https://github.com/cmake-js/cmake-js) for building. Building requires CMake >=3.2 and a C++11 compatible compiler. If using GCC it requires atleast GCC-4.9 due to needing \<regex\>.

## Using it
```javascript
// Require the library.
var Anitomy = require('anitomy');

// Construct a new Anitomy instance.
var anitomy = new Anitomy();

// Parse a filename
var success = anitomy.parse('[TaigaSubs]_Toradora!_(2008)_-_01v2_-_Tiger_and_Dragon_[1280x720_H.264_FLAC][1234ABCD].mkv');

// Get the elements
var elements = anitomy.elements();

// Output some info
console.log(elements.get('AnimeTitle') + " #" + elements.get('EpisodeNumber') + ' by ' + elements.get('ReleaseGroup'));
```

## Currently Implemented
  * Constructing Anitomy instances.
  * Parsing filenames, Note: lowercase function name 'parse' rather than 'Parse'.
  * Getting elements collection, same as Anitomy.
  * Nearly all of the elements collection functions, same as Anitomy, except for a few to do with iterators. Note: get_all returns a JS array of strings.
  * Element categories, but are represented as strings, with same names as Anitomy, but without the "anitomy::kElement" prefix, ie. 'AnimeTitle' rather than anitomy::kElementAnimeTitle, 'EpisodeNumber', 'ReleaseGroup' etc.
  * Getting options object, same as Anitomy.
  * Getting and setting the options. Note: ignored_strings is a JS array of strings, and cannot be mutated in-place.

Listing:
```javascript
new Anitomy();
Anitomy.parse(filename);
Anitomy.elements();
Anitomy.options();

Elements.empty();
Elements.empty(category);
Elements.size();
Elements.at(index);
Elements[index];
Elements.get(category);
Elements.get_all(category);
Elements.clear();
Elements.insert(category, value);
Elements.erase(category);
Elements.count(category);

Options.allowed_delimiters
Options.ignored_strings
Options.parse_episode_number
Options.parse_episode_title
Options.parse_file_extension
Options.parse_release_group
```

## To Be Implemented
  * Anitomy.tokens()

## TODO
  * Proper documentation instead of just function listing

## License
*node.anitomy.js* is licensed under [Mozilla Public License 2.0](https://www.mozilla.org/en-US/MPL/2.0/FAQ/).