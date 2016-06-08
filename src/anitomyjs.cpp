/*
** Copyright (c) 2016, Thomas Farr
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include <nan.h>
#include "objects/anitomy_obj.h"
#include "objects/elements_obj.h"

NAN_MODULE_INIT(Init) {
	objects::Anitomy::Init(target);
	objects::Elements::Init(target);
}

NODE_MODULE(anitomy, Init)