var anitomyjs = require('bindings')('anitomyjs');

var anitomy = new anitomyjs.Anitomy();
var elements = anitomy.elements();
var filename = '[Ecchihiro]_Netoge_no_Yome_wa_Onnanoko_ja_Nai_to_Omotta？_-_07-10_[720p_Hi10P_AAC][C74A839D].mkv';

console.log('filename = ' + filename);
console.log('elements.empty() = ' + elements.empty());
console.log('anitomy.Parse(filename) = ' + anitomy.Parse(filename));
console.log('elements.empty() = ' + elements.empty());
console.log('elements.size() = ' + elements.size());

for (var i = 0; i < elements.size() ; i++) {
    console.log('elements[' + i + '] = ');
    console.log(elements[i]);
}

console.log('elements.get(\'AnimeTitle\') = ' + elements.get('AnimeTitle'));
console.log('elements.get_all(\'EpisodeNumber\') = ' + elements.get_all('EpisodeNumber'))
