/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <nan.h>
#include <string>

#include "./utils.h"

NAN_METHOD(ParseSync) {
  std::string filename;
  if (!StringParam(info, 0, "filename", filename)) return;
}

NAN_MODULE_INIT(Init) {
  Nan::Export(target, "parseSync", ParseSync);
}

NODE_MODULE(anitomy_lib, Init);