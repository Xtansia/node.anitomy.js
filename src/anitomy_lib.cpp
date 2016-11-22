/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "nan_nowarn.h"
#include "anitomy_async.h"
#include "anitomy_elements.h"
#include "anitomy_sync.h"

NAN_MODULE_INIT(Init) {
  AnitomyElements::Init();
  Nan::Export(target, "parse", ParseAsync);
  Nan::Export(target, "parseSync", ParseSync);
}

NODE_MODULE(anitomy_lib, Init)
