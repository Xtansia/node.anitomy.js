var anitomyjs = require('./index.js');

var anitomy = new anitomyjs.Anitomy();
var elements = anitomy.elements();
var filename = '[Ecchihiro]_Netoge_no_Yome_wa_Onnanoko_ja_Nai_to_Omotta？_-_07-10_[720p_Hi10P_AAC][C74A839D].mkv';

console.log('------------------------------------------------------------------------------');

console.log('filename = ' + filename);
console.log('anitomy = ' + anitomy);
console.log('elements = anitomy.elements() = ' + elements);

console.log('------------------------------------------------------------------------------');

console.log('elements.empty() = ' + elements.empty());
console.log('anitomy.parse(filename) = ' + anitomy.parse(filename));
console.log('elements.empty() = ' + elements.empty());
console.log('elements.size() = ' + elements.size());

console.log('------------------------------------------------------------------------------');

for (var i = 0; i < elements.size(); i++) {
  console.log('elements[' + i + '] = ', elements[i]);
}

console.log('------------------------------------------------------------------------------');

console.log('elements.get(\'AnimeTitle\') = ' + elements.get('AnimeTitle'));
console.log('elements.get_all(\'EpisodeNumber\') = ' + elements.get_all('EpisodeNumber'));
console.log('elements.count(\'EpisodeNumber\') = ' + elements.count('EpisodeNumber'));
console.log('elements.empty(\'AnimeTitle\') = ' + elements.empty('AnimeTitle'));
console.log('elements.empty(\'AnimeYear\') = ' + elements.empty('AnimeYear'));

console.log('------------------------------------------------------------------------------');

console.log('elements.clear()'); elements.clear();
console.log('elements.empty() = ' + elements.empty());
console.log('elements.insert(\'AnimeTitle\', \'Toradora\')'); elements.insert('AnimeTitle', 'Toradora');
console.log('elements.get(\'AnimeTitle\') = ' + elements.get('AnimeTitle'));

console.log('------------------------------------------------------------------------------');