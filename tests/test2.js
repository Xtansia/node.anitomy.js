var Anitomy = require('../anitomy');
var fs = require('fs');
var path = require('path');

var anitomy = new Anitomy();
var elements = anitomy.elements();

var expected_fails = [
	'[Juuni.Kokki]-(Les.12.Royaumes)-[Ep.24]-[x264+OGG]-[JAP+FR+Sub.FR]-[Chap]-[AzF].mkv',
	'[Ayu]_Kiddy_Grade_2_-_Pilot_[H264_AC3][650B731B].mkv',
	'[Keroro].148.[Xvid.mp3].[FE68D5F1].avi',
	'Aim_For_The_Top!_Gunbuster-ep1.BD(H264.FLAC.10bit)[KAA][69ECCDCF].mkv',
	'[Mobile Suit Gundam Seed Destiny HD REMASTER][07][Big5][720p][AVC_AAC][encoded by SEED].mp4',
	'「K」 Image Blu-ray WHITE & BLACK - Main (BD 1280x720 AVC AAC).mp4',
	'【MMZYSUB】★【Golden Time】[24（END）][GB][720P_MP4]',
	'[AoJiaoZero][Mangaka-san to Assistant-san to the Animation] 02 [BIG][X264_AAC][720P].mp4',
	'[Asenshi] Rozen Maiden 3 - PV [CA57F300].mkv',
	'37 [Ruberia]_Death_Note_-_37v2_[FINAL]_[XviD][6FA7D273].avi',
	'[UTW]_Accel_World_-_EX01_[BD][h264-720p_AAC][3E56EE18].mkv',
	'EvoBot.[Watakushi]_Akuma_no_Riddle_-_01v2_[720p][69A307A2].mkv',
	'The iDOLM@STER 765 Pro to Iu Monogatari.mkv',
	'[SpoonSubs]_Hidamari_Sketch_x365_-_04.1_(DVD)[B6CE8458].mkv',
	'The.Animatrix.08.A.Detective.Story.720p.BluRay.DTS.x264-ESiR.mkv',
	'[ReDone] Memories Off 3.5 - 04 (DVD 10-bit).mkv',
	'Byousoku 5 Centimeter [Blu-Ray][1920x1080 H.264][2.0ch AAC][SOFTSUBS]'
];

fs.readFile(path.resolve(__dirname, '../deps/anitomy/test/data.json'), 'utf8', function (err, data) {
  if (err) throw err;

  var testData = JSON.parse(data);

  for (var i = 0; i < testData.length; i++) {
    var anime = testData[i];
    var fail = false;

    anitomy.parse(anime.file_name);
    if (anime.anime_title && elements.get('AnimeTitle') != anime.anime_title) {
      console.log('(' + elements.get('AnimeTitle') + ' != ' + anime.anime_title + ') : ' + anime.file_name);
      if (expected_fails.indexOf(anime.file_name) == -1) {
        fail = true;
      }
    }

    if (fail) {
      throw new Error('Unexpected parsing failures');
    }
  }
});