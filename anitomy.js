var anitomyjs = require('bindings')('anitomyjs');

var anitomy = new anitomyjs.Anitomy();
console.log("Elements are empty: " + anitomy.elements().empty())
var res = anitomy.Parse('[Ecchihiro]_Netoge_no_Yome_wa_Onnanoko_ja_Nai_to_Omotta？_-_07_[720p_Hi10P_AAC][C74A839D].mkv');
console.log("Parse result: " + res);
console.log("Elements are empty: " + anitomy.elements().empty())