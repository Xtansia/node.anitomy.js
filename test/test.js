/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

// TODO: Implement testing of option handling, and filename arrays

const anitomy = require('../anitomy');
const async = require('async');
const es = require('event-stream');
const expect = require('chai').expect;
const fs = require('fs');
const JSONStream = require('JSONStream');
const path = require('path');

const bad_file_names = [
  '[Juuni.Kokki]-(Les.12.Royaumes)-[Ep.24]-[x264+OGG]-[JAP+FR+Sub.FR]-[Chap]-[AzF].mkv',
  'Code_Geass_R2_TV_[20_of_25]_[ru_jp]_[HDTV]_[Varies_&_Cuba77_&_AnimeReactor_RU].mkv',
  'Noein_[01_of_24]_[ru_jp]_[bodlerov_&_torrents_ru].mkv',
  '[Ayu]_Kiddy_Grade_2_-_Pilot_[H264_AC3][650B731B].mkv',
  '[Keroro].148.[Xvid.mp3].[FE68D5F1].avi',
  'Macross Frontier - Sayonara no Tsubasa (Central Anime, 720p) [46B35E25].mkv',
  'Aim_For_The_Top!_Gunbuster-ep1.BD(H264.FLAC.10bit)[KAA][69ECCDCF].mkv',
  '[Mobile Suit Gundam Seed Destiny HD REMASTER][07][Big5][720p][AVC_AAC][encoded by SEED].mp4',
  '「K」 Image Blu-ray WHITE & BLACK - Main (BD 1280x720 AVC AAC).mp4',
  '[[Zero-Raws] Shingeki no Kyojin - 05 (MBS 1280x720 x264 AAC).mp4',
  'Evangelion Shin Gekijouban Q (BDrip 1920x1080 x264 FLACx2 5.1ch)-ank.mkv',
  '【MMZYSUB】★【Golden Time】[24（END）][GB][720P_MP4]',
  '[AoJiaoZero][Mangaka-san to Assistant-san to the Animation] 02 [BIG][X264_AAC][720P].mp4',
  '[Asenshi] Rozen Maiden 3 - PV [CA57F300].mkv',
  '__BLUE DROP 10 (1).avi',
  '37 [Ruberia]_Death_Note_-_37v2_[FINAL]_[XviD][6FA7D273].avi',
  '[UTW]_Accel_World_-_EX01_[BD][h264-720p_AAC][3E56EE18].mkv',
  'EvoBot.[Watakushi]_Akuma_no_Riddle_-_01v2_[720p][69A307A2].mkv',
  '01 - Land of Visible Pain.mkv',
  'The iDOLM@STER 765 Pro to Iu Monogatari.mkv',
  '[SpoonSubs]_Hidamari_Sketch_x365_-_04.1_(DVD)[B6CE8458].mkv',
  'Ep. 01 - The Boy in the Iceberg',
  'The.Animatrix.08.A.Detective.Story.720p.BluRay.DTS.x264-ESiR.mkv',
  'Neko no Ongaeshi - [HQR.remux-DualAudio][NTV.1280x692.h264](0CDC2145).mkv',
  '[ReDone] Memories Off 3.5 - 04 (DVD 10-bit).mkv',
  'Byousoku 5 Centimeter [Blu-Ray][1920x1080 H.264][2.0ch AAC][SOFTSUBS]'
];

function parseSync(filename) {
  return function () {
    return anitomy.parseSync(filename);
  };
}

function parseAsync(filename, callback) {
  return function () {
    return anitomy.parse(filename, callback);
  };
}

function streamTestData() {
  const filename = path.join(__dirname, '../lib/anitomy/test/data.json');
  const stream = fs.createReadStream(filename, {encoding: 'utf8'});
  const parser = JSONStream.parse('*');
  return stream.pipe(parser);
}

describe('Anitomy', function () {
  describe('#parse(filename, callback)', function () {
    it('should throw Error when filename is not provided', function () {
      expect(parseAsync()).to.throw('filenames must be provided');

      expect(parseAsync('')).to.not.throw('filenames must be provided');
    });

    it('should throw TypeError when filename is provided but isn\'t a string', function () {
      expect(parseAsync(null)).to.throw(TypeError, 'filenames must be a string or an array of strings');
      expect(parseAsync(10)).to.throw(TypeError, 'filenames must be a string or an array of strings');
      expect(parseAsync(false)).to.throw(TypeError, 'filenames must be a string or an array of strings');
      expect(parseAsync({})).to.throw(TypeError, 'filenames must be a string or an array of strings');

      expect(parseAsync([''])).to.not.throw(TypeError, 'filenames must be a string or an array of strings');
      expect(parseAsync('')).to.not.throw(TypeError, 'filenames must be a string or an array of strings');
    });

    it('should throw Error when callback is not provided', function () {
      expect(parseAsync('')).to.throw('callback must be provided');

      expect(parseAsync('', function(){})).to.not.throw('callback must be provided');
    });

    it('should throw TypeError when callback is provided but isn\'t a function', function () {
      expect(parseAsync('', null)).to.throw(TypeError, 'callback must be a function');
      expect(parseAsync('', 10)).to.throw(TypeError, 'callback must be a function');
      expect(parseAsync('', false)).to.throw(TypeError, 'callback must be a function');
      expect(parseAsync('', [])).to.throw(TypeError, 'callback must be a function');
      expect(parseAsync('', {})).to.throw(TypeError, 'callback must be a function');

      expect(parseAsync('', function(){})).to.not.throw(TypeError, 'callback must be a function');
    });

    it('should correctly parse some basic info from filenames', function (done) {
      streamTestData().pipe(es.map(function (data, cb) {
        if (bad_file_names.indexOf(data.file_name) !== -1) return cb();

        anitomy.parse(data.file_name, function (elems) {
          try {
            expect(elems.AnimeTitle).to.eql(data.anime_title);
            expect(elems.EpisodeNumber).to.eql(data.episode_number);
            expect(elems.FileChecksum).to.eql(data.file_checksum);
            expect(elems.ReleaseGroup).to.eql(data.release_group);
            return cb();
          } catch (err) {
            return cb(err);
          }
        });
      })).on('error', done).on('end', done);
    });

    it('should return an empty AnitomyElements when filename is empty', function (done) {
      async.each(['', '[Hello]_World_-_01.mkv'], function (title, cb) {
        anitomy.parse(title, function (elems) {
          try {
            expect(elems.empty()).to.eql(title == '');
            return cb();
          } catch (err) {
            return cb(err);
          }
        });
      }, done);
    });
  });

  describe('#parseSync(filename)', function () {
    it('should throw Error when filename is not provided', function () {
      expect(parseSync()).to.throw('filenames must be provided');

      expect(parseSync('')).to.not.throw();
    });

    it('should throw TypeError when filename is provided but isn\'t a string', function () {
      expect(parseSync(null)).to.throw(TypeError, 'filenames must be a string or an array of strings');
      expect(parseSync(10)).to.throw(TypeError, 'filenames must be a string or an array of strings');
      expect(parseSync(false)).to.throw(TypeError, 'filenames must be a string or an array of strings');
      expect(parseSync({})).to.throw(TypeError, 'filenames must be a string or an array of strings');

      expect(parseSync([''])).to.not.throw();
      expect(parseSync('')).to.not.throw();
    });

    it('should correctly parse some basic info from filenames', function (done) {
      streamTestData().pipe(es.map(function (data, cb) {
        if (bad_file_names.indexOf(data.file_name) !== -1) return cb();

        const elems = anitomy.parseSync(data.file_name);
        try {
          expect(elems.AnimeTitle).to.eql(data.anime_title);
          expect(elems.EpisodeNumber).to.eql(data.episode_number);
          expect(elems.FileChecksum).to.eql(data.file_checksum);
          expect(elems.ReleaseGroup).to.eql(data.release_group);
          return cb();
        } catch (err) {
          return cb(err);
        }
      })).on('error', done).on('end', done);
    });

    it('should return an empty AnitomyElements when filename is empty', function () {
      ['', '[Hello]_World_-_01.mkv'].forEach(function (title) {
        expect(anitomy.parseSync(title).empty()).to.eql(title == '');
      });
    });
  });
});
