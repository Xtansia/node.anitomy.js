var anitomyjs = require('bindings')('anitomyjs');

var anitomy = new anitomyjs.Anitomy();
var filename = '[Ecchihiro]_Netoge_no_Yome_wa_Onnanoko_ja_Nai_to_Omotta？_-_07-10_[720p_Hi10P_AAC][C74A839D].mkv';

console.log('filename = ' + filename);
console.log('anitomy.elements.empty() = ' + anitomy.elements.empty());
console.log('anitomy.parse(filename) = ' + anitomy.parse(filename));
console.log('anitomy.elements.empty() = ' + anitomy.elements.empty());
console.log('anitomy.elements.size() = ' + anitomy.elements.size());

for (var i = 0; i < anitomy.elements.size(); i++) {
  var pair = anitomy.elements[i];
  console.log('anitomy.elements[' + i + '] = { category: \'' + pair.category + '\', value: \'' + pair.value + '\' }');
}

console.log('anitomy.elements.get(\'AnimeTitle\') = ' + anitomy.elements.get('AnimeTitle'));
console.log('anitomy.elements.get_all(\'EpisodeNumber\') = ' + anitomy.elements.get_all('EpisodeNumber'));
console.log('anitomy.elements.count(\'EpisodeNumber\') = ' + anitomy.elements.count('EpisodeNumber'));
console.log('anitomy.elements.empty(\'AnimeTitle\') = ' + anitomy.elements.empty('AnimeTitle'));
console.log('anitomy.elements.empty(\'AnimeYear\') = ' + anitomy.elements.empty('AnimeYear'));
console.log('anitomy.elements.clear()'); anitomy.elements.clear();
console.log('anitomy.elements.empty() = ' + anitomy.elements.empty());
console.log('anitomy.elements.insert(\'AnimeTitle\', \'Toradora\')'); anitomy.elements.insert('AnimeTitle', 'Toradora');
console.log('anitomy.elements.get(\'AnimeTitle\') = ' + anitomy.elements.get('AnimeTitle'));

var elems = new anitomyjs.Elements();
console.log('elems.insert(\'AnimeTitle\', \'Kiznaiver\')'); elems.insert('AnimeTitle', 'Kiznaiver');
console.log('elems.get(\'AnimeTitle\') = ' + elems.get('AnimeTitle'));
console.log('elems.erase(\'AnimeTitle\')'); elems.erase('AnimeTitle');
console.log('elems.empty(\'AnimeTitle\') = ' + elems.empty('AnimeTitle'));